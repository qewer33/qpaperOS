#pragma once

#include "apps.h"
#include "resources/app_icons.h"

class AppGpsSync : public App {
public:
  using App::App;
  void drawUI(GxEPD_Class *display) override;
};

extern std::unique_ptr<AppGpsSync> appGpsSync;