#pragma once

#include "apps.h"
#include "resources/app_icons.h"

class AppWiFiSmartconfig : public App {
public:
  using App::App;
  void drawUI(GxEPD_Class *display) override;
};

extern std::unique_ptr<AppWiFiSmartconfig> appWiFiSmartconfig;