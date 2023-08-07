#include "apps.h"

#include "apps/wifi_smartconfig/app_wifi_smartconfig.h"

std::vector<std::unique_ptr<App>> apps;
unsigned int currentAppIndex = 0;

App::App(String name, const unsigned char *icon, unsigned int iconWidth, unsigned int iconHeight) {
  this->name = name;
  this->icon = icon;
  this->iconWidth = iconWidth;
  this->iconHeight = iconHeight;
}

void App::setup() {}
void App::drawUI(GxEPD_Class *display) {}
void App::exit() {}
void App::buttonClick() {}
void App::buttonDoubleClick() {}

void initApps() { apps.emplace_back(std::move(appWiFiSmartconfig)); }

void drawAppsListUI(GxEPD_Class *display, ESP32Time *rtc, int batteryStatus) {
  display->fillScreen(GxEPD_WHITE);
  display->setTextColor(GxEPD_BLACK);
  display->setTextWrap(false);

  // Time
  String hoursFiller = rtc->getHour(true) < 10 ? "0" : "";
  String minutesFiller = rtc->getMinute() < 10 ? "0" : "";
  String timeStr = hoursFiller + String(rtc->getHour(true)) + ":" + minutesFiller + String(rtc->getMinute());
  display->setFont(&Outfit_60011pt7b);
  printLeftString(display, timeStr.c_str(), 11, 22);

  // Battery
  printRightString(display, String(String(batteryStatus) + "%").c_str(), 166, 22);

  const unsigned char *icon_battery_small_array[6] = {epd_bitmap_icon_battery_0_small,  epd_bitmap_icon_battery_20_small,
                                                      epd_bitmap_icon_battery_40_small, epd_bitmap_icon_battery_60_small,
                                                      epd_bitmap_icon_battery_80_small, epd_bitmap_icon_battery_100_small};
  display->drawBitmap(170, 2, icon_battery_small_array[batteryStatus / 20], 28, 28, GxEPD_BLACK);

  // App
  display->drawRoundRect(46, 46, 108, 108, 10, GxEPD_BLACK);
  display->drawRoundRect(45, 45, 110, 110, 11, GxEPD_BLACK);
  display->drawBitmap(50, 50, apps[currentAppIndex]->icon, 100, 100, GxEPD_BLACK);
  printCenterString(display, apps[currentAppIndex]->name.c_str(), 100, 185);
}