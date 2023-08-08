#pragma once

#include "Arduino.h"
#include "ESP32Time.h"
#include "GxDEPG0150BN/GxDEPG0150BN.h" // 1.54" b/w 200x200
#include "GxEPD.h"
#include "memory"
#include "vector"

#include "lib/ui.h"

#include "resources/fonts/Outfit_60011pt7b.h"
#include "resources/icons.h"

class App {
public:
  String name;
  const unsigned char *icon;
  unsigned int iconWidth;
  unsigned int iconHeight;

  App(String name, const unsigned char *icon);
  virtual void setup();
  virtual void drawUI(GxEPD_Class *display);
  virtual void exit();
  virtual void buttonClick();
  virtual void buttonDoubleClick();
};

extern std::vector<std::unique_ptr<App>> apps;

extern uint32_t currentAppIndex;

void initApps();
void drawAppsListUI(GxEPD_Class *display, ESP32Time *rtc, int batteryStatus);