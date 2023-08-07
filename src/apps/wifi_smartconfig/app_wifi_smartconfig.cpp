#include "app_wifi_smartconfig.h"

void AppWiFiSmartconfig::drawUI(GxEPD_Class *display) {
  display->fillScreen(GxEPD_WHITE);
  display->updateWindow(0, 0, GxEPD_WIDTH, GxEPD_HEIGHT);
}

std::unique_ptr<AppWiFiSmartconfig> appWiFiSmartconfig(new AppWiFiSmartconfig("WiFi Smartconfig", icon_app_wifi_smartconfig, 30, 30));