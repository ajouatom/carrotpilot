#include <cmath>
#include <filesystem>
#include <unordered_set>

#include "selfdrive/frogpilot/ui/visual_settings.h"
#include "selfdrive/ui/ui.h"

FrogPilotVisualsPanel::FrogPilotVisualsPanel(SettingsWindow *parent) : ListWidget(parent) {
  isMetric = params.getBool("IsMetric");

  backButton = new ButtonControl(tr(""), tr("BACK"));
  connect(backButton, &ButtonControl::clicked, [=]() {
    hideSubToggles();
  });
  addItem(backButton);

  const std::vector<std::tuple<QString, QString, QString, QString>> visualToggles {
    {"CustomTheme", "Custom Themes", "Enable the ability to use custom themes.", "../frogpilot/assets/wheel_images/frog.png"},
    {"CustomColors", "Custom Colors", "This toggle lets you replace the standard openpilot color scheme with your choice of color schemes.\n\nWant to submit your own color scheme? Post it in the 'feature-request' channel on the FrogPilot Discord!", ""},
    {"CustomIcons", "Custom Icons", "Switch out the default openpilot icons with a set of custom icons of your choice.\n\nWant to submit your own color scheme? Post it in the 'feature-request' channel on the FrogPilot Discord!", ""},
    {"CustomSignals", "Custom Signals", "Enable custom animations for your turn signals, adding a personal touch to the signal indicators.\n\nWant to submit your own color scheme? Post it in the 'feature-request' channel on the FrogPilot Discord!", ""},
    {"CustomSounds", "Custom Sounds", "Use this to replace the standard openpilot sounds with a set of custom sounds for a unique auditory experience.\n\nWant to submit your own color scheme? Post it in the 'feature-request' channel on the FrogPilot Discord!", ""},

    {"CameraView", "Camera View (Cosmetic Only)", "Choose your preferred camera view for the onroad UI. This is a visual change only and doesn't affect how openpilot uses the cameras.", "../frogpilot/assets/toggle_icons/icon_camera.png"},
    {"Compass", "Compass", "Add a compass to your onroad UI to easily see the direction you're driving in.", "../frogpilot/assets/toggle_icons/icon_compass.png"},

    {"CustomUI", "Custom Onroad UI", "Customize the look and feel of your Onroad UI, making the driving interface suit your style.", "../assets/offroad/icon_road.png"},
    {"AdjacentPath", "Adjacent Paths", "Display paths to the left and right of your car, visualizing where the system detects lanes and assists in anticipating lane changes.", ""},
    {"BlindSpotPath", "Blind Spot Path", "Visualize your blind spots with a red path when another vehicle is detected nearby.", ""},
    {"ShowFPS", "FPS Counter", "Display the Frames Per Second (FPS) of your onroad UI for monitoring system performance.", ""},
    {"LeadInfo", "Lead Info and Logics", "Get detailed information about the vehicle ahead, including speed and distance, and the logic behind your following distance.", ""},
    {"RoadNameUI", "Road Name", "See the name of the road you're on at the bottom of your screen, sourced directly from OpenStreetMap.", ""},
    {"RotatingWheel", "Rotating Steering Wheel", "The on-screen steering wheel icon mirrors your real steering wheel's movements.", ""},

    {"DriverCamera", "Driver Camera On Reverse", "Automatically show the driver's camera feed when you shift to reverse.", "../assets/img_driver_face_static.png"},
    {"GreenLightAlert", "Green Light Alert", "Get an alert when a traffic light changes from red to green, so you're always ready to move.", "../frogpilot/assets/toggle_icons/icon_green_light.png"},

    {"ModelUI", "Model UI", "Personalize how the model's visualizations appear on your screen, tailoring it to your preference.", "../assets/offroad/icon_calibration.png"},
    {"AccelerationPath", "Acceleration Path", "Visualize the car's intended acceleration or deceleration with a color-coded path.", ""},
    {"LaneLinesWidth", "Lane Lines", "Adjust the visual thickness of lane lines on your display.\n\nDefault matches the MUTCD average of 4 inches.", ""},
    {"PathEdgeWidth", "Path Edges", "Adjust the width of the path edges shown on your UI, which represent different driving modes and statuses.\n\nDefault is 20% of the total path.\n\nBlue = Navigation\nLight Blue = Always On Lateral\nGreen = Default with 'FrogPilot Colors'\nLight Green = Default with stock colors\nOrange = Experimental Mode Active\nYellow = Conditional Overriden", ""},
    {"PathWidth", "Path Width", "Customize the width of the driving path shown on your UI to match the actual width of your car, enhancing spatial awareness.\n\nDefault matches the width of a 2019 Lexus ES 350.", ""},
    {"RoadEdgesWidth", "Road Edges", "Adjust the visual thickness of road edges on your display.\n\nDefault is 1/2 of the MUTCD average lane line width of 4 inches.", ""},
    {"UnlimitedLength", "'Unlimited' Road UI Length", "Extend the display of the path, lane lines, and road edges as far as the system can detect, providing a more expansive view of the road ahead.", ""},

    {"ScreenBrightness", "Screen Brightness", "Customize your screen brightness beyond the standard auto setting for optimal visibility in all conditions.", "../frogpilot/assets/toggle_icons/icon_light.png"},
    {"SilentMode", "Silent Mode", "Mute all openpilot sounds for a quieter driving experience, ideal when you need silence or are listening to something important.", "../frogpilot/assets/toggle_icons/icon_mute.png"},
    {"WheelIcon", "Steering Wheel Icon", "Replace the default steering wheel icon with a custom design, adding a unique touch to your interface.", "../assets/offroad/icon_openpilot.png"},
  };

  for (const auto &[param, title, desc, icon] : visualToggles) {
    ToggleControl *toggle;
    if (param == "CustomTheme") {
      ParamManageControl *customThemeToggle = new ParamManageControl(param, title, desc, icon, this);
      connect(customThemeToggle, &ParamManageControl::manageButtonClicked, this, [this](){
        backButton->setVisible(true);
        for (auto &[key, toggle] : toggles) {
          toggle->setVisible(customThemeKeys.find(key.c_str()) != customThemeKeys.end());
        }
      });
      toggle = customThemeToggle;
    } else if (param == "CustomColors" || param == "CustomIcons" || param == "CustomSignals" || param == "CustomSounds") {
      toggle = new ParamValueControl(param, title, desc, icon, 0, 3, {{0, "Stock"}, {1, "Frog"}, {2, "Tesla"}, {3, "Stalin"}}, this);

    } else if (param == "CameraView") {
      toggle = new ParamValueControl(param, title, desc, icon, 0, 3, {{0, "Auto"}, {1, "Standard"}, {2, "Wide"}, {3, "Driver"}}, this);

    } else if (param == "CustomUI") {
      ParamManageControl *customUIToggle = new ParamManageControl(param, title, desc, icon, this);
      connect(customUIToggle, &ParamManageControl::manageButtonClicked, this, [this](){
        backButton->setVisible(true);
        for (auto &[key, toggle] : toggles) {
          toggle->setVisible(customOnroadUIKeys.find(key.c_str()) != customOnroadUIKeys.end());
        }
      });
      toggle = customUIToggle;

    } else if (param == "ModelUI") {
      ParamManageControl *modelUIToggle = new ParamManageControl(param, title, desc, icon, this);
      connect(modelUIToggle, &ParamManageControl::manageButtonClicked, this, [this](){
        backButton->setVisible(true);
        for (auto &[key, toggle] : toggles) {
          toggle->setVisible(modelUIKeys.find(key.c_str()) != modelUIKeys.end());
        }
      });
      toggle = modelUIToggle;
    } else if (param == "LaneLinesWidth" || param == "RoadEdgesWidth") {
      toggle = new ParamValueControl(param, title, desc, icon, 0, 24, [](int value) {return QString::number(value) + " inches";}, this);
    } else if (param == "PathEdgeWidth") {
      toggle = new ParamValueControl(param, title, desc, icon, 0, 100, [](int value) {return QString::number(value) + "%";}, this);
    } else if (param == "PathWidth") {
      toggle = new ParamValueControl(param, title, desc, icon, 0, 100, [](int value) {return QString::number(value / 10.0) + " feet";}, this);

    } else if (param == "ScreenBrightness") {
      auto brightnessLabelGenerator = [](int value) -> QString {return value == 0 ? "Screen Off" : value == 101 ? "Auto" : QString::number(value) + "%";};
      toggle = new ParamValueControl(param, title, desc, icon, 0, 101, brightnessLabelGenerator, this);

    } else if (param == "WheelIcon") {
      std::map<int, QString> steeringWheelLabels = {{0, "Stock"}, {1, "Lexus"}, {2, "Toyota"}, {3, "Frog"}, {4, "Rocket"}, {5, "Hyundai"}, {6, "Stalin"}};
      toggle = new ParamValueControl(param, title, desc, icon, 0, 6, steeringWheelLabels, this);

    } else {
      toggle = new ParamControl(param, title, desc, icon, this);
    }

    addItem(toggle);
    toggles[param.toStdString()] = toggle;

    std::unordered_set<std::string> rebootParams = {"RoadNameUI"};
    connect(toggle, &ToggleControl::toggleFlipped, this, [this, rebootParams, currentParam = param.toStdString()]() {
      if (rebootParams.find(currentParam) != rebootParams.end()) {
        if (ConfirmationDialog::toggle("Reboot required to take effect.", "Reboot Now", this)) {
          Hardware::reboot();
        }
      }
      paramsMemory.putBool("FrogPilotTogglesUpdated", true);
    });

    if (ParamValueControl *paramValueToggle = static_cast<ParamValueControl *>(toggle)) {
      connect(paramValueToggle, &ParamValueControl::valueChanged, this, [this]() {
        if (toggles.find("ScreenBrightness") != toggles.end()) {
          uiState()->scene.screen_brightness = params.getInt("ScreenBrightness");
        }

        paramsMemory.putBool("FrogPilotTogglesUpdated", true);
      });
    }
  }

  customOnroadUIKeys = {"AdjacentPath", "BlindSpotPath", "ShowFPS", "LeadInfo", "RoadNameUI", "RotatingWheel"};
  customThemeKeys = {"CustomColors", "CustomIcons", "CustomSignals", "CustomSounds"};
  modelUIKeys = {"AccelerationPath", "LaneLinesWidth", "PathEdgeWidth", "PathWidth", "RoadEdgesWidth", "UnlimitedLength"};

  QObject::connect(uiState(), &UIState::uiUpdate, this, &FrogPilotVisualsPanel::updateMetric);

  hideSubToggles();
  setDefaults();
  updateMetric();
}

void FrogPilotVisualsPanel::updateMetric() {
  std::thread([&] {
    if (isVisible()) return;

    previousIsMetric = isMetric;
    isMetric = params.getBool("IsMetric");

    if (previousIsMetric == isMetric) return;

    const auto refreshToggleControl = [](ToggleControl *control) {
      if (ParamManageControl *paramManageControl = dynamic_cast<ParamManageControl*>(control)) {
        paramManageControl->refresh();
      } else if (ParamValueControl *paramValueControl = dynamic_cast<ParamValueControl*>(control)) {
        paramValueControl->refresh();
      }
    };

    const auto updateLabelGeneratorForControl = [](ToggleControl *control, int minVal, int maxVal, std::function<QString(int)> labelFormatter) {
      if (ParamValueControl *paramValueControl = dynamic_cast<ParamValueControl*>(control)) {
        paramValueControl->updateLabelGenerator(minVal, maxVal, labelFormatter);
      }
  };

    if (isMetric != previousIsMetric) {
      const double distanceConversion = isMetric ? INCH_TO_CM : CM_TO_INCH;
      const double speedConversion = isMetric ? FOOT_TO_METER : METER_TO_FOOT;
      params.putInt("LaneLinesWidth", std::nearbyint(params.getInt("LaneLinesWidth") * distanceConversion));
      params.putInt("RoadEdgesWidth", std::nearbyint(params.getInt("RoadEdgesWidth") * distanceConversion));
      params.putInt("PathWidth", std::nearbyint(params.getInt("PathWidth") * speedConversion));
    }

    ToggleControl *laneLinesWidthToggle = toggles["LaneLinesWidth"];
    ToggleControl *roadEdgesWidthToggle = toggles["RoadEdgesWidth"];
    ToggleControl *pathWidthToggle = toggles["PathWidth"];

    if (isMetric) {
      laneLinesWidthToggle->setDescription("Customize the lane line width.\n\nDefault matches the Vienna average of 10 centimeters.");
      roadEdgesWidthToggle->setDescription("Customize the road edges width.\n\nDefault is 1/2 of the Vienna average lane line width of 10 centimeters.");
      updateLabelGeneratorForControl(laneLinesWidthToggle, 0, 60, [](int value) { return QString::number(value) + " centimeters"; });
      updateLabelGeneratorForControl(roadEdgesWidthToggle, 0, 60, [](int value) { return QString::number(value) + " centimeters"; });
      updateLabelGeneratorForControl(pathWidthToggle, 0, 30, [](int value) { return QString::number(value / 10.0) + " meters"; });
    } else {
      laneLinesWidthToggle->setDescription("Customize the lane line width.\n\nDefault matches the MUTCD average of 4 inches.");
      roadEdgesWidthToggle->setDescription("Customize the road edges width.\n\nDefault is 1/2 of the MUTCD average lane line width of 4 inches.");
      updateLabelGeneratorForControl(laneLinesWidthToggle, 0, 24, [](int value) { return QString::number(value) + " inches"; });
      updateLabelGeneratorForControl(roadEdgesWidthToggle, 0, 24, [](int value) { return QString::number(value) + " inches"; });
      updateLabelGeneratorForControl(pathWidthToggle, 0, 100, [](int value) { return QString::number(value / 10.0) + " feet"; });
    }

    refreshToggleControl(toggles["LaneLinesWidth"]);
    refreshToggleControl(toggles["RoadEdgesWidth"]);
    refreshToggleControl(toggles["PathWidth"]);

    previousIsMetric = isMetric;
  }).detach();
}

void FrogPilotVisualsPanel::hideSubToggles() {
  backButton->setVisible(false);

  for (auto &[key, toggle] : toggles) {
    const bool subToggles = modelUIKeys.find(key.c_str()) != modelUIKeys.end() || 
                            customOnroadUIKeys.find(key.c_str()) != customOnroadUIKeys.end() ||
                            customThemeKeys.find(key.c_str()) != customThemeKeys.end();
    toggle->setVisible(!subToggles);
  }
}

void FrogPilotVisualsPanel::hideEvent(QHideEvent *event) {
  hideSubToggles();
}

void FrogPilotVisualsPanel::setDefaults() {
  const bool FrogsGoMoo = params.get("DongleId").substr(0, 3) == "be6";

  const std::map<std::string, std::string> defaultValues {
    {"AccelerationPath", "1"},
    {"AdjacentPath", FrogsGoMoo ? "1" : "0"},
    {"BlindSpotPath", "1"},
    {"CameraView", FrogsGoMoo ? "1" : "0"},
    {"Compass", FrogsGoMoo ? "1" : "0"},
    {"CustomColors", "1"},
    {"CustomIcons", "1"},
    {"CustomSignals", "1"},
    {"CustomSounds", "1"},
    {"CustomTheme", "1"},
    {"CustomUI", "1"},
    {"DriverCamera", "0"},
    {"GreenLightAlert", "0"},
    {"LaneLinesWidth", "4"},
    {"LeadInfo", FrogsGoMoo ? "1" : "0"},
    {"ModelUI", "1"},
    {"NumericalTemp", "0"},
    {"PathEdgeWidth", "20"},
    {"PathWidth", "61"},
    {"RoadEdgesWidth", "2"},
    {"RoadNameUI", "1"},
    {"RotatingWheel", "1"},
    {"ScreenBrightness", "101"},
    {"ShowCPU", FrogsGoMoo ? "1" : "0"},
    {"ShowMemoryUsage", FrogsGoMoo ? "1" : "0"},
    {"ShowFPS", FrogsGoMoo ? "1" : "0"},
    {"Sidebar", FrogsGoMoo ? "1" : "0"},
    {"SilentMode", "0"},
    {"UnlimitedLength", "1"},
    {"WheelIcon", FrogsGoMoo ? "1" : "0"},
  };

  bool rebootRequired = false;
  for (const auto &[key, value] : defaultValues) {
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
