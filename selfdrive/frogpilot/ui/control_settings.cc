#include <cmath>
#include <filesystem>
#include <unordered_set>

#include "selfdrive/frogpilot/ui/control_settings.h"
#include "selfdrive/ui/ui.h"

FrogPilotControlsPanel::FrogPilotControlsPanel(SettingsWindow *parent) : ListWidget(parent) {
  isMetric = params.getBool("IsMetric");

  backButton = new ButtonControl(tr(""), tr("BACK"));
  connect(backButton, &ButtonControl::clicked, [=]() {
    hideSubToggles();
  });
  addItem(backButton);

  const std::vector<std::tuple<QString, QString, QString, QString>> controlToggles {
    {"AdjustablePersonalities", "Adjustable Personalities", "Use the 'Distance' button on the steering wheel or the onroad UI to switch between openpilot's driving personalities.\n\n1 bar = Aggressive\n2 bars = Standard\n3 bars = Relaxed", "../frogpilot/assets/toggle_icons/icon_distance.png"},

    {"AlwaysOnLateral", "Always on Lateral", "Maintain openpilot lateral control when the brake or gas pedals are used.\n\n1Deactivation occurs only through the 'Cruise Control' button.", "../frogpilot/assets/toggle_icons/icon_always_on_lateral.png"},
    {"AlwaysOnLateralMain", "   Enable 'Always on Lateral' On Cruise Main", "Activate 'Always On Lateral' when cruise control is engaged without needing openpilot to be enabled first.", "../frogpilot/assets/toggle_icons/icon_blank.png"},

    {"ConditionalExperimental", "Conditional Experimental Mode", "Automatically 'Experimental Mode' engages under predefined conditions.", "../frogpilot/assets/toggle_icons/icon_conditional.png"},
    {"CESpeed", "When Driving Below", "Switch to 'Experimental Mode' below this speed in absence of a lead vehicle.", ""},
    {"CESpeedLead", "When Driving Below With Lead", "Switch to 'Experimental Mode' below this speed when following a lead vehicle.", ""},
    {"CECurves", "Curve Detected Ahead", "Switch to 'Experimental Mode' when a curve is detected.", ""},
    {"CECurvesLead", "Curve Detected Ahead With Lead", "Switch to 'Experimental Mode' when a curve is detected and following a lead vehicle.", ""},
    {"CENavigation", "Navigation Based", "Switch to 'Experimental Mode' based on navigation data. (i.e. Intersections, stop signs, etc.)", ""},
    {"CESlowerLead", "Slower Lead Detected Ahead", "Switch to 'Experimental Mode' when a slower lead vehicle is detected ahead.", ""},
    {"CEStopLights", "Stop Lights and Stop Signs", "Switch to 'Experimental Mode' when a stop light or stop sign is detected.", ""},
    {"CESignal", "Turn Signal When Driving Below 55 mph", "Switch to 'Experimental Mode' when using turn signals below 55 mph to help assit with turns.", ""},

    {"CustomPersonalities", "Custom Driving Personalities", "Customize the driving personality profiles to your preferences.", "../frogpilot/assets/toggle_icons/icon_custom.png"},
    {"AggressiveFollow", "Follow Time", "Set the 'Aggressive Personality' following distance. Represents seconds to follow the lead vehicle.\n\nStock: 1.25 seconds.", "../frogpilot/assets/other_images/aggressive.png"},
    {"AggressiveJerk", "Jerk Value", "Configure brake/gas pedal responsiveness for the 'Aggressive Personality'. Higher values yield a more 'relaxed' response.\n\nStock: 0.5.", "../frogpilot/assets/other_images/aggressive.png"},
    {"StandardFollow", "Follow Time", "Set the 'Standard Personality' following distance. Represents seconds to follow the lead vehicle.\n\nStock: 1.45 seconds.", "../frogpilot/assets/other_images/standard.png"},
    {"StandardJerk", "Jerk Value", "Adjust brake/gas pedal responsiveness for the 'Standard Personality'. Higher values yield a more 'relaxed' response.\n\nStock: 1.0.", "../frogpilot/assets/other_images/standard.png"},
    {"RelaxedFollow", "Follow Time", "Set the 'Relaxed Personality' following distance. Represents seconds to follow the lead vehicle.\n\nStock: 1.75 seconds.", "../frogpilot/assets/other_images/relaxed.png"},
    {"RelaxedJerk", "Jerk Value", "Set brake/gas pedal responsiveness for the 'Relaxed Personality'. Higher values yield a more 'relaxed' response.\n\nStock: 1.0.", "../frogpilot/assets/other_images/relaxed.png"},

    {"DeviceShutdown", "Device Shutdown Timer", "Configure the timer for automatic device shutdown when offroad conserving energy and preventing battery drain.", "../frogpilot/assets/toggle_icons/icon_time.png"},
    {"ExperimentalModeViaPress", "Experimental Mode Via 'LKAS' Button / Screen", "Toggle Experimental Mode by double-clicking 'Lane Departure'/'LKAS' button or double tapping screen.\n\nOverrides 'Conditional Experimental Mode'.", "../assets/img_experimental_white.svg"},

    {"FireTheBabysitter", "Fire the Babysitter", "Deactivate certain openpilot 'Babysitter Protocols' for more user autonomy.", "../frogpilot/assets/toggle_icons/icon_babysitter.png"},
    {"NoLogging", "Disable All Logging", "Turn off all data tracking to enhance privacy or reduce thermal load.\n\nWARNING: This action will prevent drive recording and data cannot be recovered!", ""},
    {"MuteDM", "Mute Driver Monitoring", "Disable driver monitoring.", ""},
    {"MuteDoor", "Mute Door Open Alert", "Disable alerts for open doors.", ""},
    {"MuteOverheated", "Mute Overheated System Alert", "Disable alerts for device overheated.", ""},
    {"MuteSeatbelt", "Mute Seatbelt Unlatched Alert", "Disable alerts for unlatched seatbelt.", ""},

    {"LateralTune", "Lateral Tuning", "Modify openpilot's steering behavior.", "../frogpilot/assets/toggle_icons/icon_lateral_tune.png"},
    {"AverageCurvature", "Average Desired Curvature", "Use Pfeiferj's distance-based curvature adjustment for improved curve handling.", ""},
    {"NNFF", "NNFF - Neural Network Feedforward", "Use Twilsonco's Neural Network Feedforward for enhanced precision in lateral control.", ""},

    {"LongitudinalTune", "Longitudinal Tuning", "Modify openpilot's acceleration and braking behavior.", "../frogpilot/assets/toggle_icons/icon_longitudinal_tune.png"},
    {"AccelerationProfile", "Acceleration Profile", "Change the acceleration rate to be either sporty or eco-friendly.", ""},
    {"AggressiveAcceleration", "Aggressive Acceleration With Lead", "Increase acceleration aggressiveness when following a lead vehicle from a stop.", ""},
    {"SmoothBraking", "Smoother Braking Behind Lead", "Enable smoother braking when approaching a slower vehicle.", ""},
    {"StoppingDistance", "Increased Stopping Distance", "Increase the stopping distance for a more comfortable stop.", ""},

    {"Model", "Model Selector", "Choose your preferred openpilot model.", "../assets/offroad/icon_calibration.png"},
    {"MTSCEnabled", "Map Turn Speed Control", "Slow down for anticipated curves detected by your downloaded maps.", "../frogpilot/assets/toggle_icons/icon_speed_map.png"},

    {"NudgelessLaneChange", "Nudgeless Lane Change", "Enable lane changes without manual steering input.", "../frogpilot/assets/toggle_icons/icon_lane.png"},
    {"LaneChangeTime", "Lane Change Timer", "Specify a delay before executing a nudgeless lane change.", ""},
    {"LaneDetection", "Lane Detection", "Block nudgeless lane changes when a lane isn't detected.", ""},
    {"OneLaneChange", "One Lane Change Per Signal", "Limit to one nudgeless lane change per turn signal activation.", ""},
    {"PauseLateralOnSignal", "Pause Lateral On Turn Signal", "Temporarily disable lateral control during turn signal use.", ""},

    {"SpeedLimitController", "Speed Limit Controller", "Automatically adjust vehicle speed to match speed limits using 'Open Street Map's, 'Navigate On openpilot', or your car's dashboard (Toyota only).", "../assets/offroad/icon_speed_limit.png"},
    {"Offset1", "Speed Limit Offset (0-34 mph)", "Set speed limit offset for limits between 0-34 mph.", ""},
    {"Offset2", "Speed Limit Offset (35-54 mph)", "Set speed limit offset for limits between 35-54 mph.", ""},
    {"Offset3", "Speed Limit Offset (55-64 mph)", "Set speed limit offset for limits between 55-64 mph.", ""},
    {"Offset4", "Speed Limit Offset (65-99 mph)", "Set speed limit offset for limits between 65-99 mph.", ""},
    {"SLCFallback", "Fallback Method", "Choose a fallback method for unregistered speed limits in Navigation, OSM, or the car's dashboard.", ""},
    {"SLCPriority", "Speed Limit Priority", "Determine priority order for selecting speed limits with 'Speed Limit Controller'.", ""},

    {"TurnDesires", "Use Turn Desires", "Engage turn desires for enhanced precision in turns below minimum lane change speed.", "../assets/navigation/direction_continue_right.png"},

    {"VisionTurnControl", "Vision Turn Speed Controller", "Slow down for detected road curvature for smoother curve handling.", "../frogpilot/assets/toggle_icons/icon_vtc.png"},
    {"CurveSensitivity", "Curve Detection Sensitivity", "Set curve detection sensitivity. Higher values prompt earlier responses, lower values lead to smoother but later reactions.", ""},
    {"TurnAggressiveness", "Turn Speed Aggressiveness", "Set turn speed aggressiveness. Higher values result in faster turns, lower values yield gentler turns.", ""},
  };

  for (const auto &[param, title, desc, icon] : controlToggles) {
    ToggleControl *toggle;
    if (param == "AdjustablePersonalities") {
      toggle = new ParamValueControl(param, title, desc, icon, 0, 3, {{0, "None"}, {1, "Steering Wheel"}, {2, "Onroad UI Button"}, {3, "Wheel + Button"}}, this);

    } else if (param == "ConditionalExperimental") {
      ParamManageControl *conditionalExperimentalToggle = new ParamManageControl(param, title, desc, icon, this);
      connect(conditionalExperimentalToggle, &ParamManageControl::manageButtonClicked, this, [this](){
        backButton->setVisible(true);
        modelSelectorButton->setVisible(false);
        for (auto &[key, toggle] : toggles) {
          toggle->setVisible(conditionalExperimentalKeys.find(key.c_str()) != conditionalExperimentalKeys.end());
        }
      });
      toggle = conditionalExperimentalToggle;
    } else if (param == "CESpeed" || param == "CESpeedLead") {
      toggle = new ParamValueControl(param, title, desc, icon, 0, 99, [](int value) {return QString::number(value) + " mph";}, this);

    } else if (param == "CustomPersonalities") {
      ParamManageControl *customPersonalitiesToggle = new ParamManageControl(param, title, desc, icon, this);
      connect(customPersonalitiesToggle, &ParamManageControl::manageButtonClicked, this, [this](){
        backButton->setVisible(true);
        modelSelectorButton->setVisible(false);
        for (auto &[key, toggle] : toggles) {
          toggle->setVisible(customPersonalitiesKeys.find(key.c_str()) != customPersonalitiesKeys.end());
        }
      });
      toggle = customPersonalitiesToggle;
    } else if (param == "AggressiveFollow" || param == "StandardFollow" || param == "RelaxedFollow") {
      toggle = new ParamValueControl(param, title, desc, icon, 8, 50, [](int value) {return QString::number(value / 10.0) + " seconds";}, this);
    } else if (param == "AggressiveJerk" || param == "StandardJerk" || param == "RelaxedJerk") {
      toggle = new ParamValueControl(param, title, desc, icon, 1, 50, [](int value) {return QString::number(value / 10.0);}, this);

    } else if (param == "DeviceShutdown") {
      auto shutdownLabelGenerator = [](int value) -> QString {
        return value == 0 ? "Instant" : value <= 3 ? QString::number(value * 15) + " mins" : QString::number(value - 3) + (value == 4 ? " hour" : " hours");
      };
      toggle = new ParamValueControl(param, title, desc, icon, 0, 33, shutdownLabelGenerator, this);

    } else if (param == "FireTheBabysitter") {
      ParamManageControl *fireTheBabysitterToggle = new ParamManageControl(param, title, desc, icon, this);
      connect(fireTheBabysitterToggle, &ParamManageControl::manageButtonClicked, this, [this](){
        backButton->setVisible(true);
        modelSelectorButton->setVisible(false);
        for (auto &[key, toggle] : toggles) {
          toggle->setVisible(fireTheBabysitterKeys.find(key.c_str()) != fireTheBabysitterKeys.end());
        }
      });
      toggle = fireTheBabysitterToggle;

    } else if (param == "LateralTune") {
      ParamManageControl *lateralTuneToggle = new ParamManageControl(param, title, desc, icon, this);
      connect(lateralTuneToggle, &ParamManageControl::manageButtonClicked, this, [this](){
        backButton->setVisible(true);
        modelSelectorButton->setVisible(false);
        for (auto &[key, toggle] : toggles) {
          toggle->setVisible(lateralTuneKeys.find(key.c_str()) != lateralTuneKeys.end());
        }
      });
      toggle = lateralTuneToggle;

    } else if (param == "LongitudinalTune") {
      ParamManageControl *longitudinalTuneToggle = new ParamManageControl(param, title, desc, icon, this);
      connect(longitudinalTuneToggle, &ParamManageControl::manageButtonClicked, this, [this](){
        backButton->setVisible(true);
        modelSelectorButton->setVisible(false);
        for (auto &[key, toggle] : toggles) {
          toggle->setVisible(longitudinalTuneKeys.find(key.c_str()) != longitudinalTuneKeys.end());
        }
      });
      toggle = longitudinalTuneToggle;
    } else if (param == "AccelerationProfile") {
      toggle = new ParamValueControl(param, title, desc, icon, 0, 2, {{0, "Standard"}, {1, "Eco"}, {2, "Sport"}}, this);
    } else if (param == "StoppingDistance") {
      toggle = new ParamValueControl(param, title, desc, icon, 0, 10, [](int value) {return QString::number(value) + " feet";}, this);

    } else if (param == "Model") {
      modelSelectorButton = new ButtonIconControl(tr("Model Selector"), tr("SELECT"), tr("Select your preferred openpilot model."), "../assets/offroad/icon_calibration.png");
      const QStringList models = {"Blue Diamond V2", "Blue Diamond V1", "Farmville", "New Lemon Pie", "New Delhi"};
      connect(modelSelectorButton, &ButtonIconControl::clicked, this, [this, models](){
        const int currentModel = params.getInt("Model");
        const QString currentModelLabel = models[currentModel];

        const QString selection = MultiOptionDialog::getSelection(tr("Select a driving model"), models, currentModelLabel, this);
        if (!selection.isEmpty()) {
          const int selectedModel = models.indexOf(selection);
          params.putInt("Model", selectedModel);
          params.remove("CalibrationParams");
          params.remove("LiveTorqueParameters");
          modelSelectorButton->setValue(selection);
          if (ConfirmationDialog::toggle("Reboot required to take effect.", "Reboot Now", this)) {
            Hardware::reboot();
          }
        }
      });
      modelSelectorButton->setValue(models[params.getInt("Model")]);
      addItem(modelSelectorButton);

    } else if (param == "NudgelessLaneChange") {
      ParamManageControl *laneChangeToggle = new ParamManageControl(param, title, desc, icon, this);
      connect(laneChangeToggle, &ParamManageControl::manageButtonClicked, this, [this](){
        backButton->setVisible(true);
        modelSelectorButton->setVisible(false);
        for (auto &[key, toggle] : toggles) {
          toggle->setVisible(laneChangeKeys.find(key.c_str()) != laneChangeKeys.end());
        }
      });
      toggle = laneChangeToggle;
    } else if (param == "LaneChangeTime") {
      auto laneChangeTimeLabelGenerator = [](int value) -> QString {return value == 0 ? "Instant" : QString::number(value / 2.0) + " seconds";};
      toggle = new ParamValueControl(param, title, desc, icon, 0, 10, laneChangeTimeLabelGenerator, this);

    } else if (param == "SpeedLimitController") {
      ParamManageControl *speedLimitControllerToggle = new ParamManageControl(param, title, desc, icon, this);
      connect(speedLimitControllerToggle, &ParamManageControl::manageButtonClicked, this, [this](){
        backButton->setVisible(true);
        modelSelectorButton->setVisible(false);
        for (auto &[key, toggle] : toggles) {
          toggle->setVisible(speedLimitControllerKeys.find(key.c_str()) != speedLimitControllerKeys.end());
        }
      });
      toggle = speedLimitControllerToggle;
    } else if (param == "Offset1" || param == "Offset2" || param == "Offset3" || param == "Offset4") {
      toggle = new ParamValueControl(param, title, desc, icon, 0, 99, [](int value) {return QString::number(value) + " mph";}, this);
    } else if (param == "SLCFallback") {
      toggle = new ParamValueControl(param, title, desc, icon, 0, 2, {{0, "None"}, {1, "Experimental Mode"}, {2, "Previous Speed Limit"}}, this);
    } else if (param == "SLCPriority") {
      auto slcPriorityLabelGenerator = [](int priority) -> QString {
        return priority == 0 ? "Navigation, Dash, OSM" :
               priority == 1 ? "Navigation, OSM, Dash" :
               priority == 2 ? "Navigation, OSM" :
               priority == 3 ? "Navigation, Dash" :
               priority == 4 ? "Navigation" :
               priority == 5 ? "OSM, Dash, Navigation" :
               priority == 6 ? "OSM, Navigation, Dash" :
               priority == 7 ? "OSM, Navigation" :
               priority == 8 ? "OSM, Dash" :
               priority == 9 ? "OSM" :
               priority == 10 ? "Dash, Navigation, OSM" :
               priority == 11 ? "Dash, OSM, Navigation" :
               priority == 12 ? "Dash, OSM" :
               priority == 13 ? "Dash, Navigation" :
               priority == 14 ? "Dash" :
               priority == 15 ? "Highest" :
               "Lowest";
      };
      toggle = new ParamValueControl(param, title, desc, icon, 0, 16, slcPriorityLabelGenerator, this);

    } else if (param == "VisionTurnControl") {
      ParamManageControl *visionTurnControlToggle = new ParamManageControl(param, title, desc, icon, this);
      connect(visionTurnControlToggle, &ParamManageControl::manageButtonClicked, this, [this](){
        backButton->setVisible(true);
        modelSelectorButton->setVisible(false);
        for (auto &[key, toggle] : toggles) {
          toggle->setVisible(visionTurnControlKeys.find(key.c_str()) != visionTurnControlKeys.end());
        }
      });
      toggle = visionTurnControlToggle;
    } else if (param == "CurveSensitivity" || param == "TurnAggressiveness") {
      toggle = new ParamValueControl(param, title, desc, icon, 1, 200, [](int value) {return QString::number(value) + "%";}, this);

    } else {
      toggle = new ParamControl(param, title, desc, icon, this);
    }

    addItem(toggle);
    toggles[param.toStdString()] = toggle;

    std::unordered_set<std::string> rebootParams = {"AlwaysOnLateral", "AlwaysOnLateralMain", "FireTheBabysitter", "MuteDM", "NoLogging", "NNFF", "SpeedLimitController"};
    connect(toggle, &ToggleControl::toggleFlipped, this, [this, rebootParams, currentParam = param.toStdString()]() {
      ToggleControl *alwaysOnLateralMainToggle = toggles["AlwaysOnLateralMain"];
      if (currentParam == "AlwaysOnLateral") {
        alwaysOnLateralMainToggle->setVisible(params.getBool("AlwaysOnLateral"));
      }

      if (currentParam == "NNFF") {
        if (params.getBool("NNFF")) {
          const bool addSSH = ConfirmationDialog::yesorno("Would you like to grant 'twilsonco' SSH access to improve NNFF? This won't affect any added SSH keys.", this);
          params.putBool("TwilsoncoSSH", addSSH);
          if (addSSH) {
            ConfirmationDialog::toggleAlert("Message 'twilsonco' on Discord to get your device properly configured.", "Acknowledge", this);
          }
        }
      }

      if (rebootParams.find(currentParam) != rebootParams.end()) {
        if (ConfirmationDialog::toggle("Reboot required to take effect.", "Reboot Now", this)) {
          Hardware::reboot();
        }
      }
      paramsMemory.putBool("FrogPilotTogglesUpdated", true);
    });

    if (ParamValueControl *paramValueToggle = static_cast<ParamValueControl *>(toggle)) {
      connect(paramValueToggle, &ParamValueControl::valueChanged, this, [this]() {
        paramsMemory.putBool("FrogPilotTogglesUpdated", true);
      });
    }
  }

  conditionalExperimentalKeys = {"CESpeed", "CESpeedLead", "CECurves", "CECurvesLead", "CESlowerLead", "CENavigation", "CEStopLights", "CESignal"};
  customPersonalitiesKeys = {"AggressiveFollow", "AggressiveJerk", "StandardFollow", "StandardJerk", "RelaxedFollow", "RelaxedJerk"};
  fireTheBabysitterKeys = {"NoLogging", "MuteDM", "MuteDoor", "MuteOverheated", "MuteSeatbelt"};
  laneChangeKeys = {"LaneChangeTime", "LaneDetection", "OneLaneChange", "PauseLateralOnSignal"};
  lateralTuneKeys = {"AverageCurvature", "NNFF"};
  longitudinalTuneKeys = {"AccelerationProfile", "AggressiveAcceleration", "SmoothBraking", "StoppingDistance"};
  speedLimitControllerKeys = {"Offset1", "Offset2", "Offset3", "Offset4", "SLCFallback", "SLCPriority"};
  visionTurnControlKeys = {"CurveSensitivity", "TurnAggressiveness"};

  QObject::connect(uiState(), &UIState::uiUpdate, this, &FrogPilotControlsPanel::updateMetric);

  hideSubToggles();
  setDefaults();
  updateMetric();
}

void FrogPilotControlsPanel::updateMetric() {
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

    const auto updateTitleAndDescription = [](ToggleControl *control, const QString &title, const QString &desc) {
      if (ParamValueControl *paramValueControl = dynamic_cast<ParamValueControl*>(control)) {
        paramValueControl->updateTitle(title);
        control->setDescription(desc);
      }
    };

    if (isMetric != previousIsMetric) {
      const double distanceConversion = isMetric ? FOOT_TO_METER : METER_TO_FOOT;
      const double speedConversion = isMetric ? MILE_TO_KM : KM_TO_MILE;
      params.putInt("CESpeed", std::nearbyint(params.getInt("CESpeed") * speedConversion));
      params.putInt("CESpeedLead", std::nearbyint(params.getInt("CESpeedLead") * speedConversion));
      params.putInt("Offset1", std::nearbyint(params.getInt("Offset1") * speedConversion));
      params.putInt("Offset2", std::nearbyint(params.getInt("Offset2") * speedConversion));
      params.putInt("Offset3", std::nearbyint(params.getInt("Offset3") * speedConversion));
      params.putInt("Offset4", std::nearbyint(params.getInt("Offset4") * speedConversion));
      params.putInt("StoppingDistance", std::nearbyint(params.getInt("StoppingDistance") * distanceConversion));
    }

    ToggleControl *ceSpeedToggle = toggles["CESpeed"];
    ToggleControl *ceSpeedLeadToggle = toggles["CESpeedLead"];
    ToggleControl *ceSignalToggle = toggles["CESignal"];
    ToggleControl *offset1Toggle = toggles["Offset1"];
    ToggleControl *offset2Toggle = toggles["Offset2"];
    ToggleControl *offset3Toggle = toggles["Offset3"];
    ToggleControl *offset4Toggle = toggles["Offset4"];
    ToggleControl *stoppingDistanceToggle = toggles["StoppingDistance"];

    if (isMetric) {
      updateLabelGeneratorForControl(ceSpeedToggle, 0, 150, [](int value) { return QString::number(value) + " kph"; });
      updateLabelGeneratorForControl(ceSpeedLeadToggle, 0, 150, [](int value) { return QString::number(value) + " kph"; });
      updateLabelGeneratorForControl(offset1Toggle, 0, 150, [](int value) { return QString::number(value) + " kph"; });
      updateLabelGeneratorForControl(offset2Toggle, 0, 150, [](int value) { return QString::number(value) + " kph"; });
      updateLabelGeneratorForControl(offset3Toggle, 0, 150, [](int value) { return QString::number(value) + " kph"; });
      updateLabelGeneratorForControl(offset4Toggle, 0, 150, [](int value) { return QString::number(value) + " kph"; });
      updateLabelGeneratorForControl(stoppingDistanceToggle, 0, 5, [](int value) { return QString::number(value) + " meters"; });
      updateTitleAndDescription(ceSignalToggle, "Turn Signal When Driving Below 90 kph", "Switch to 'Experimental Mode' when using turn signals below 90 kph to help assit with turns.");
      updateTitleAndDescription(offset1Toggle, "Speed Limit Offset (0-34 kph)", "Set speed limit offset for limits between 0-34 kph.");
      updateTitleAndDescription(offset2Toggle, "Speed Limit Offset (35-54 kph)", "Set speed limit offset for limits between 35-54 kph.");
      updateTitleAndDescription(offset3Toggle, "Speed Limit Offset (55-64 kph)", "Set speed limit offset for limits between 55-64 kph.");
      updateTitleAndDescription(offset4Toggle, "Speed Limit Offset (65-99 kph)", "Set speed limit offset for limits between 65-99 kph.");
    } else {
      updateLabelGeneratorForControl(ceSpeedToggle, 0, 99, [](int value) { return QString::number(value) + " mph"; });
      updateLabelGeneratorForControl(ceSpeedLeadToggle, 0, 99, [](int value) { return QString::number(value) + " mph"; });
      updateLabelGeneratorForControl(offset1Toggle, 0, 99, [](int value) { return QString::number(value) + " mph"; });
      updateLabelGeneratorForControl(offset2Toggle, 0, 99, [](int value) { return QString::number(value) + " mph"; });
      updateLabelGeneratorForControl(offset3Toggle, 0, 99, [](int value) { return QString::number(value) + " mph"; });
      updateLabelGeneratorForControl(offset4Toggle, 0, 99, [](int value) { return QString::number(value) + " mph"; });
      updateLabelGeneratorForControl(stoppingDistanceToggle, 0, 10, [](int value) { return QString::number(value) + " feet"; });
      updateTitleAndDescription(ceSignalToggle, "Turn Signal When Driving Below 55 mph", "Switch to 'Experimental Mode' when using turn signals below 55 mph to help assit with turns.");
      updateTitleAndDescription(offset1Toggle, "Speed Limit Offset (0-34 mph)", "Set speed limit offset for limits between 0-34 mph.");
      updateTitleAndDescription(offset2Toggle, "Speed Limit Offset (35-54 mph)", "Set speed limit offset for limits between 35-54 mph.");
      updateTitleAndDescription(offset3Toggle, "Speed Limit Offset (55-64 mph)", "Set speed limit offset for limits between 55-64 mph.");
      updateTitleAndDescription(offset4Toggle, "Speed Limit Offset (65-99 mph)", "Set speed limit offset for limits between 65-99 mph.");
    }

    refreshToggleControl(toggles["CESpeed"]);
    refreshToggleControl(toggles["CESpeedLead"]);
    refreshToggleControl(toggles["CESignal"]);
    refreshToggleControl(toggles["Offset1"]);
    refreshToggleControl(toggles["Offset2"]);
    refreshToggleControl(toggles["Offset3"]);
    refreshToggleControl(toggles["Offset4"]);
    refreshToggleControl(toggles["StoppingDistance"]);

    previousIsMetric = isMetric;
  }).detach();
}

void FrogPilotControlsPanel::hideSubToggles() {
  backButton->setVisible(false);
  modelSelectorButton->setVisible(true);

  for (auto &[key, toggle] : toggles) {
    const bool subToggles = conditionalExperimentalKeys.find(key.c_str()) != conditionalExperimentalKeys.end() ||
                            customPersonalitiesKeys.find(key.c_str()) != customPersonalitiesKeys.end() ||
                            fireTheBabysitterKeys.find(key.c_str()) != fireTheBabysitterKeys.end() ||
                            laneChangeKeys.find(key.c_str()) != laneChangeKeys.end() ||
                            lateralTuneKeys.find(key.c_str()) != lateralTuneKeys.end() ||
                            longitudinalTuneKeys.find(key.c_str()) != longitudinalTuneKeys.end() ||
                            speedLimitControllerKeys.find(key.c_str()) != speedLimitControllerKeys.end() ||
                            visionTurnControlKeys.find(key.c_str()) != visionTurnControlKeys.end();
    toggle->setVisible(!subToggles);
  }
}

void FrogPilotControlsPanel::hideEvent(QHideEvent *event) {
  hideSubToggles();
}

void FrogPilotControlsPanel::setDefaults() {
  const bool FrogsGoMoo = params.get("DongleId").substr(0, 3) == "be6";

  const std::map<std::string, std::string> defaultValues {
    {"AccelerationProfile", "0"},
    {"AdjustablePersonalities", "3"},
    {"AggressiveAcceleration", "1"},
    {"AggressiveFollow", FrogsGoMoo ? "10" : "12"},
    {"AggressiveJerk", FrogsGoMoo ? "6" : "5"},
    {"AlwaysOnLateral", "1"},
    {"AlwaysOnLateralMain", FrogsGoMoo ? "1" : "0"},
    {"AverageCurvature", FrogsGoMoo ? "1" : "0"},
    {"CECurves", "1"},
    {"CECurvesLead", "0"},
    {"CENavigation", "1"},
    {"CESignal", "1"},
    {"CESlowerLead", "0"},
    {"CESpeed", "0"},
    {"CESpeedLead", "0"},
    {"CEStopLights", "1"},
    {"ConditionalExperimental", "1"},
    {"CurveSensitivity", FrogsGoMoo ? "125" : "100"},
    {"DeviceShutdown", "9"},
    {"ExperimentalModeViaPress", "1"},
    {"FireTheBabysitter", FrogsGoMoo ? "1" : "0"},
    {"LaneChangeTime", "0"},
    {"LaneDetection", "1"},
    {"LateralTune", "1"},
    {"LongitudinalTune", "1"},
    {"MuteDM", FrogsGoMoo ? "1" : "0"},
    {"MuteDoor", FrogsGoMoo ? "1" : "0"},
    {"MuteOverheated", FrogsGoMoo ? "1" : "0"},
    {"MuteSeatbelt", FrogsGoMoo ? "1" : "0"},
    {"NNFF", FrogsGoMoo ? "1" : "0"},
    {"NudgelessLaneChange", "1"},
    {"Offset1", "5"},
    {"Offset2", FrogsGoMoo ? "7" : "5"},
    {"Offset3", "10"},
    {"Offset4", FrogsGoMoo ? "20" : "10"},
    {"OneLaneChange", "1"},
    {"PauseLateralOnSignal", "0"},
    {"RelaxedFollow", "30"},
    {"RelaxedJerk", "50"},
    {"SLCFallback", "2"},
    {"SLCPriority", "1"},
    {"SmoothBraking", "1"},
    {"SpeedLimitController", "1"},
    {"StandardFollow", "15"},
    {"StandardJerk", "10"},
    {"StoppingDistance", FrogsGoMoo ? "6" : "3"},
    {"TurnAggressiveness", FrogsGoMoo ? "150" : "100"},
    {"TurnDesires", "1"},
    {"VisionTurnControl", "1"},
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
