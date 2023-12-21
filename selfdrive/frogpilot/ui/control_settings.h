#pragma once

#include <set>

#include "selfdrive/ui/qt/offroad/settings.h"

class FrogPilotControlsPanel : public ListWidget {
  Q_OBJECT

public:
  explicit FrogPilotControlsPanel(SettingsWindow *parent);

private:
  void hideEvent(QHideEvent *event);
  void hideSubToggles();
  void setDefaults();
  void updateMetric();

  ButtonControl *backButton;
  ButtonIconControl *modelSelectorButton;

  std::set<QString> conditionalExperimentalKeys;
  std::set<QString> customPersonalitiesKeys;
  std::set<QString> fireTheBabysitterKeys;
  std::set<QString> laneChangeKeys;
  std::set<QString> lateralTuneKeys;
  std::set<QString> longitudinalTuneKeys;
  std::set<QString> speedLimitControllerKeys;
  std::set<QString> visionTurnControlKeys;

  std::map<std::string, ToggleControl*> toggles;

  bool isMetric;
  bool previousIsMetric;

  Params params;
  Params paramsMemory{"/dev/shm/params"};
};