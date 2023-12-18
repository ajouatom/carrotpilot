#pragma once

#include <set>

#include "selfdrive/ui/qt/offroad/settings.h"

class FrogPilotVisualsPanel : public ListWidget {
  Q_OBJECT

public:
  explicit FrogPilotVisualsPanel(SettingsWindow *parent);

private:
  void hideEvent(QHideEvent *event);
  void showCustomOnroadUI(bool visible);
  void showCustomThemes(bool visible);
  void showModelUI(bool visible);
  void updateMetric();

  ButtonControl *backButton;

  std::set<QString> customOnroadUIKeys;
  std::set<QString> customThemeKeys;
  std::set<QString> modelUIKeys;

  std::map<std::string, ToggleControl*> toggles;

  bool isMetric;

  Params params;
  Params paramsMemory{"/dev/shm/params"};
};
