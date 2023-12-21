#include <filesystem>
#include <QDir>
#include <QRegularExpression>
#include <QTextStream>

#include "selfdrive/frogpilot/ui/vehicle_settings.h"
#include "selfdrive/ui/ui.h"

QStringList getCarNames(const QString &dirPath, const QString &carMake) {
  QMap<QString, QString> makeMap;
  makeMap["acura"] = "honda";
  makeMap["audi"] = "volkswagen";
  makeMap["buick"] = "gm";
  makeMap["cadillac"] = "gm";
  makeMap["chevrolet"] = "gm";
  makeMap["chrysler"] = "chrysler";
  makeMap["dodge"] = "chrysler";
  makeMap["ford"] = "ford";
  makeMap["gm"] = "gm";
  makeMap["gmc"] = "gm";
  makeMap["genesis"] = "hyundai";
  makeMap["honda"] = "honda";
  makeMap["hyundai"] = "hyundai";
  makeMap["infiniti"] = "nissan";
  makeMap["jeep"] = "chrysler";
  makeMap["kia"] = "hyundai";
  makeMap["lexus"] = "toyota";
  makeMap["lincoln"] = "ford";
  makeMap["man"] = "volkswagen";
  makeMap["mazda"] = "mazda";
  makeMap["nissan"] = "nissan";
  makeMap["ram"] = "chrysler";
  makeMap["seat"] = "volkswagen";
  makeMap["subaru"] = "subaru";
  makeMap["tesla"] = "tesla";
  makeMap["toyota"] = "toyota";
  makeMap["volkswagen"] = "volkswagen";
  makeMap["skoda"] = "volkswagen";

  QStringList names;
  QDir dir(dirPath);
  QString lowerCaseCarMake = carMake.toLower();

  QString targetFolder = makeMap.value(lowerCaseCarMake, lowerCaseCarMake);

  foreach (const QString &folder, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
    if (folder.toLower() == targetFolder) {
      QFile file(dirPath + "/" + folder + "/values.py");
      if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QRegularExpression regex("class CAR\\(StrEnum\\):([\\s\\S]*?)(?=^\\w)", QRegularExpression::MultilineOption);
        QRegularExpressionMatch match = regex.match(QTextStream(&file).readAll());
        file.close();

        if (match.hasMatch()) {
          QRegularExpression nameRegex("=\\s*\"([^\"]+)\"");
          QRegularExpressionMatchIterator it = nameRegex.globalMatch(match.captured(1));
          while (it.hasNext()) {
            names << it.next().captured(1);
          }
        }
      }
    }
  }

  std::sort(names.begin(), names.end());
  return names;
}

FrogPilotVehiclesPanel::FrogPilotVehiclesPanel(SettingsWindow *parent) : ListWidget(parent) {
  selectMakeButton = new ButtonControl(tr("Select Make"), tr("SELECT"));
  connect(selectMakeButton, &ButtonControl::clicked, [this]() {
    std::string currentModel = params.get("CarMake");
    QStringList makes = {
      "Acura", "Audi", "BMW", "Buick", "Cadillac", "Chevrolet", "Chrysler", "Dodge", "Ford", "GM", "GMC", 
      "Genesis", "Honda", "Hyundai", "Infiniti", "Jeep", "Kia", "Lexus", "Lincoln", "MAN", "Mazda", 
      "Mercedes", "Nissan", "Ram", "SEAT", "Subaru", "Tesla", "Toyota", "Volkswagen", "Volvo", "Škoda",
    };

    QString newMakeSelection = MultiOptionDialog::getSelection(tr("Select a Make"), makes, QString::fromStdString(currentModel), this);
    if (!newMakeSelection.isEmpty()) {
      params.put("CarMake", newMakeSelection.toStdString());
      selectMakeButton->setValue(newMakeSelection);
      setModels();
      setToggles();
    }
  });
  addItem(selectMakeButton);

  ButtonControl *selectModelButton = new ButtonControl(tr("Select Model"), tr("SELECT"));
  QString modelSelection = QString::fromStdString(params.get("CarModel"));
  connect(selectModelButton, &ButtonControl::clicked, [this, selectModelButton]() {
    std::string currentModel = params.get("CarModel");
    QString newModelSelection = MultiOptionDialog::getSelection(tr("Select a Model"), models, QString::fromStdString(currentModel), this);
    if (!newModelSelection.isEmpty()) {
      params.put("CarModel", newModelSelection.toStdString());
      selectModelButton->setValue(newModelSelection);
    }
  });
  selectModelButton->setValue(modelSelection);
  addItem(selectModelButton);

  noToggles = new QLabel(tr("No additional options available for the selected make."));
  noToggles->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  noToggles->setAlignment(Qt::AlignCenter);
  addItem(noToggles);

  std::vector<std::tuple<QString, QString, QString, QString>> vehicle_toggles {
    {"EVTable", "EV Lookup Tables", "Smoothens out the gas and brake controls for EV vehicles.", ""},
    {"LongPitch", "Long Pitch Compensation", "Reduces speed and acceleration error for greater passenger comfort and improved vehicle efficiency.", ""},
    {"LowerVolt", "Lower Volt Enable Speed", "Lowers the Volt's minimum enable speed in order to enable openpilot at any speed.", ""},

    {"LockDoors", "Lock Doors In Drive", "Automatically locks the doors when in drive and unlocks when in park.", ""},
    {"SNGHack", "Stop and Go Hack", "Enable the 'Stop and Go' hack for vehicles without stock stop and go functionality.", ""},
    {"TSS2Tune", "TSS2 Tune", "Tuning profile based on the tuning profile from DragonPilot for TSS2 vehicles.", ""}
  };

  for (auto &[param, title, desc, icon] : vehicle_toggles) {
    auto toggle = new ParamControl(param, title, desc, icon, this);
    addItem(toggle);
    toggles[param.toStdString()] = toggle;

    connect(toggles["TSS2Tune"], &ToggleControl::toggleFlipped, [=]() {
      if (ConfirmationDialog::toggle("Reboot required to take effect.", "Reboot Now", parent)) {
        Hardware::reboot();
      }
    });

    connect(toggle, &ToggleControl::toggleFlipped, [this]() {
      paramsMemory.putBool("FrogPilotTogglesUpdated", true);
    });
  }

  QObject::connect(uiState(), &UIState::offroadTransition, this, &FrogPilotVehiclesPanel::setToggles);

  setModels();
  setToggles();
}

void FrogPilotVehiclesPanel::setModels() {
  std::string carMake = params.get("CarMake");
  QString dirPath = "../../selfdrive/car";
  models = getCarNames(dirPath, QString::fromStdString(carMake));
}

void FrogPilotVehiclesPanel::setToggles() {
  const QString makeSelection = QString::fromStdString(params.get("CarMake"));

  if (previousMakeSelection == makeSelection) return;

  previousMakeSelection = makeSelection;
  selectMakeButton->setValue(makeSelection);

  const bool gm = makeSelection == "Buick" || makeSelection == "Cadillac" || makeSelection == "Chevrolet" || makeSelection == "GM" || makeSelection == "GMC";
  const bool toyota = makeSelection == "Lexus" || makeSelection == "Toyota";

  auto evtable = toggles["EVTable"];
  evtable->setVisible(gm);

  auto longPitch = toggles["LongPitch"];
  longPitch->setVisible(gm);

  auto lowerVolt = toggles["LowerVolt"];
  lowerVolt->setVisible(gm);

  auto lockDoors = toggles["LockDoors"];
  lockDoors->setVisible(toyota);

  auto sngHack = toggles["SNGHack"];
  sngHack->setVisible(toyota);

  auto tss2Tune = toggles["TSS2Tune"];
  tss2Tune->setVisible(toyota);

  noToggles->setVisible(!(gm || toyota));

  this->update();
}

void FrogPilotVehiclesPanel::setDefaults() {
  const bool FrogsGoMoo = params.get("DongleId").substr(0, 3) == "be6";

  const std::map<std::string, std::string> default_values {
    {"EVTable", FrogsGoMoo ? "0" : "1"},
    {"LongPitch", FrogsGoMoo ? "0" : "1"},
    {"LowerVolt", FrogsGoMoo ? "0" : "1"},
    {"LockDoors", "0"},
    {"SNGHack", "0"},
    {"TSS2Tune", "1"},
  };

  bool rebootRequired = false;
  for (const auto &[key, value] : default_values) {
    if (params.get(key).empty()) {
      params.put(key, value);
      rebootRequired = true;
    }
  }

  if (rebootRequired) {
    while (!std::filesystem::exists("/data/openpilot/prebuilt")) {
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    Hardware::reboot();
  }
}