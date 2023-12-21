#pragma once

#include <QStringList>

#include "selfdrive/ui/qt/offroad/settings.h"

class FrogPilotVehiclesPanel : public ListWidget {
  Q_OBJECT

public:
  explicit FrogPilotVehiclesPanel(SettingsWindow *parent);

private:
  void setDefaults();
  void setModels();
  void setToggles();

  ButtonControl *selectMakeButton;

  QLabel *noToggles;

  QString previousMakeSelection;
  QStringList models;

  std::map<std::string, ParamControl*> toggles;

  Params params;
  Params paramsMemory{"/dev/shm/params"};
};
