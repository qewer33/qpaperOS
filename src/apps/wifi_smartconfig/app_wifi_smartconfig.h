#pragma once

#include "Preferences.h"
#include "WiFi.h"

#include "apps.h"
#include "lib/ui.h"
#include "os_config.h"
#include "resources/app_icons.h"
#include "resources/fonts/Outfit_60011pt7b.h"

class AppWiFiSmartconfig : public App {
public:
  bool done;
  Preferences preferences;
  using App::App;
  void setup();
  void drawUI(GxEPD_Class *display) override;
  void exit();
};

extern std::unique_ptr<AppWiFiSmartconfig> appWiFiSmartconfig;