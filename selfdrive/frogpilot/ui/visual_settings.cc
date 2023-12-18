#include <cmath>

#include "selfdrive/frogpilot/ui/visual_settings.h"
#include "selfdrive/ui/ui.h"

FrogPilotVisualsPanel::FrogPilotVisualsPanel(SettingsWindow *parent) : ListWidget(parent) {
  isMetric = params.getBool("IsMetric");

  backButton = new ButtonControl(tr(""), tr("BACK"));
  connect(backButton, &ButtonControl::clicked, [=]() {
    showCustomOnroadUI(false);
    showCustomThemes(false);
    showModelUI(false);
  });
  addItem(backButton);
  backButton->setVisible(false);

  std::vector<std::tuple<QString, QString, QString, QString>> visualToggles {
    {"CustomTheme", "Custom Theme", "Enable the ability to use custom themes.", "../frogpilot/assets/wheel_images/frog.png"},
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
        showCustomThemes(true);
      });
      toggle = customThemeToggle;
    } else if (param == "CustomColors" || param == "CustomIcons" || param == "CustomSignals" || param == "CustomSounds") {
      toggle = new ParamValueControl(param, title, desc, icon, 0, 3, {{0, "Stock"}, {1, "Frog"}, {2, "Tesla"}, {3, "Stalin"}}, this);

    } else if (param == "CameraView") {
      toggle = new ParamValueControl(param, title, desc, icon, 0, 3, {{0, "Auto"}, {1, "Standard"}, {2, "Wide"}, {3, "Driver"}}, this);
    } else if (param == "Compass") {
      toggle = new ParamControl(param, title, desc, icon, this);

    } else if (param == "CustomUI") {
      ParamManageControl *customUIToggle = new ParamManageControl(param, title, desc, icon, this);
      connect(customUIToggle, &ParamManageControl::manageButtonClicked, this, [this](){
        showCustomOnroadUI(true);
      });
      toggle = customUIToggle;
    } else if (param == "AdjacentPath" || param == "BlindSpotPath" || param == "ShowFPS" || param == "LeadInfo" || param == "RoadNameUI") {
      toggle = new ParamControl(param, title, desc, icon, this);
    } else if (param == "RotatingWheel") {
      toggle = new ParamControl(param, title, desc, icon, this);

    } else if (param == "ScreenBrightness") {
      auto brightnessLabelGenerator = [](int value) -> QString {return value == 0 ? "Screen Off" : value == 101 ? "Auto" : QString::number(value) + "%";};
      toggle = new ParamValueControl(param, title, desc, icon, 0, 101, brightnessLabelGenerator, this);

    } else if (param == "ModelUI") {
      ParamManageControl *modelUIToggle = new ParamManageControl(param, title, desc, icon, this);
      connect(modelUIToggle, &ParamManageControl::manageButtonClicked, this, [this](){
        showModelUI(true);
      });
      toggle = modelUIToggle;
    } else if (param == "AccelerationPath" || param == "UnlimitedLength") {
      toggle = new ParamControl(param, title, desc, icon, this);
    } else if (param == "LaneLinesWidth" || param == "RoadEdgesWidth") {
      if (isMetric) {
        toggle = new ParamValueControl(param, title, desc, icon, 0, 60, [](int value) {return QString::number(value) + " centimeters";}, this);
      } else {
        toggle = new ParamValueControl(param, title, desc, icon, 0, 24, [](int value) {return QString::number(value) + " inches";}, this);
      }
    } else if (param == "PathEdgeWidth") {
      toggle = new ParamValueControl(param, title, desc, icon, 0, 100, [](int value) {return QString::number(value) + "%";}, this);
    } else if (param == "PathWidth") {
      if (isMetric) {
        toggle = new ParamValueControl(param, title, desc, icon, 0, 30, [](int value) {return QString::number(value / 10.0) + " meters";}, this);
      } else {
        toggle = new ParamValueControl(param, title, desc, icon, 0, 100, [](int value) {return QString::number(value / 10.0) + " feet";}, this);
      }

    } else if (param == "WheelIcon") {
      std::map<int, QString> steeringWheelLabels = {{0, "Stock"}, {1, "Lexus"}, {2, "Toyota"}, {3, "Frog"}, {4, "Rocket"}, {5, "Hyundai"}, {6, "Stalin"}};
      toggle = new ParamValueControl(param, title, desc, icon, 0, 6, steeringWheelLabels, this);
    } else {
      toggle = new ParamControl(param, title, desc, icon, this);
    }
    addItem(toggle);
    toggles[param.toStdString()] = toggle;

    connect(toggle, &ToggleControl::toggleFlipped, this, [this](){
      paramsMemory.putBool("FrogPilotTogglesUpdated", true);
    });

    if (auto *paramValueToggle = static_cast<ParamValueControl *>(toggle)) {
      connect(paramValueToggle, &ParamValueControl::valueChanged, this, [this]() {
        paramsMemory.putBool("FrogPilotTogglesUpdated", true);
      });
    }
  }

  customOnroadUIKeys = {"AdjacentPath", "BlindSpotPath", "ShowFPS", "LeadInfo", "RoadNameUI", "RotatingWheel"};
  customThemeKeys = {"CustomColors", "CustomIcons", "CustomSignals", "CustomSounds"};
  modelUIKeys = {"AccelerationPath", "LaneLinesWidth", "PathEdgeWidth", "PathWidth", "RoadEdgesWidth", "UnlimitedLength"};

  QObject::connect(uiState(), &UIState::uiUpdate, this, &FrogPilotVisualsPanel::updateMetric);

  showCustomOnroadUI(false);
  showCustomThemes(false);
  showModelUI(false);
}

void FrogPilotVisualsPanel::updateMetric() {
  if (!isVisible()) return;

  static bool previousIsMetric = isMetric;
  isMetric = params.getBool("IsMetric");

  auto refreshToggleControl = [](ToggleControl *control) {
    if (auto *paramManageControl = dynamic_cast<ParamManageControl*>(control)) {
      paramManageControl->refresh();
    } else if (auto *paramValueControl = dynamic_cast<ParamValueControl*>(control)) {
      paramValueControl->refresh();
    }
  };

  auto updateLabelGeneratorForControl = [](ToggleControl *control, int minVal, int maxVal, std::function<QString(int)> labelFormatter) {
    if (auto *paramValueControl = dynamic_cast<ParamValueControl*>(control)) {
      paramValueControl->updateLabelGenerator(minVal, maxVal, labelFormatter);
    }
  };

  if (isMetric != previousIsMetric) {
    const double conversion = isMetric ? 2.54 : 1.0 / 2.54;
    params.putInt("LaneLinesWidth", std::nearbyint(params.getInt("LaneLinesWidth") * conversion));
    params.putInt("RoadEdgesWidth", std::nearbyint(params.getInt("RoadEdgesWidth") * conversion));
    params.putInt("PathWidth", std::nearbyint(params.getInt("PathWidth") * (isMetric ? 0.3048 : 1.0 / 0.3048)));

    auto laneLinesWidthToggle = toggles["LaneLinesWidth"];
    auto roadEdgesWidthToggle = toggles["RoadEdgesWidth"];
    auto pathWidthToggle = toggles["PathWidth"];

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
  }
}

void FrogPilotVisualsPanel::showCustomOnroadUI(bool visible) {
  backButton->setVisible(visible);

  for (auto &[key, toggle] : toggles) {
    if (customOnroadUIKeys.find(key.c_str()) != customOnroadUIKeys.end()) {
      toggle->setVisible(visible);
    } else if (!(customThemeKeys.find(key.c_str()) != customThemeKeys.end() || modelUIKeys.find(key.c_str()) != modelUIKeys.end())) {
      toggle->setVisible(!visible);
    }
  }
}

void FrogPilotVisualsPanel::showCustomThemes(bool visible) {
  backButton->setVisible(visible);

  for (auto &[key, toggle] : toggles) {
    if (customThemeKeys.find(key.c_str()) != customThemeKeys.end()) {
      toggle->setVisible(visible);
    } else if (!(customOnroadUIKeys.find(key.c_str()) != customOnroadUIKeys.end() || modelUIKeys.find(key.c_str()) != modelUIKeys.end())) {
      toggle->setVisible(!visible);
    }
  }
}

void FrogPilotVisualsPanel::showModelUI(bool visible) {
  backButton->setVisible(visible);

  for (auto &[key, toggle] : toggles) {
    if (modelUIKeys.find(key.c_str()) != modelUIKeys.end()) {
      toggle->setVisible(visible);
    } else if (!(customOnroadUIKeys.find(key.c_str()) != customOnroadUIKeys.end() || customThemeKeys.find(key.c_str()) != customThemeKeys.end())) {
      toggle->setVisible(!visible);
    }
  }
}

void FrogPilotVisualsPanel::hideEvent(QHideEvent *event) {
  showCustomOnroadUI(false);
  showCustomThemes(false);
  showModelUI(false);
}
