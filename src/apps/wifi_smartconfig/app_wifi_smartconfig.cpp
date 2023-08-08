#include "app_wifi_smartconfig.h"

void AppWiFiSmartconfig::setup() {
  done = false;
  preferences.begin(PREFS_KEY);
}

void AppWiFiSmartconfig::drawUI(GxEPD_Class *display) {
  display->fillScreen(GxEPD_WHITE);
  display->setTextColor(GxEPD_BLACK);
  display->setFont(&Outfit_60011pt7b);
  display->drawBitmap(50, 20, icon_app_wifi_smartconfig, 100, 100, GxEPD_BLACK);

  int timer = 0;

  if (!done) {
    printCenterString(display, "Waiting connection...", 100, 150);
    display->updateWindow(0, 0, GxEPD_WIDTH, GxEPD_HEIGHT);

    WiFi.disconnect();
    WiFi.mode(WIFI_AP_STA);
    WiFi.beginSmartConfig();

    while (timer < 500) {
      timer++;
      delay(100);

      if (WiFi.smartConfigDone()) {
        display->fillRect(0, 130, GxEPD_WIDTH, 50, GxEPD_WHITE);
        printCenterString(display, String("Connected!").c_str(), 100, 150);
        printCenterString(display, WiFi.SSID().c_str(), 100, 175);
        display->updateWindow(0, 0, GxEPD_WIDTH, GxEPD_HEIGHT);
        preferences.putString("wifi_ssid", WiFi.SSID());
        preferences.putString("wifi_passwd", WiFi.psk());
        break;
      }
    }

    if (timer >= 500) {
      display->fillRect(0, 130, GxEPD_WIDTH, 50, GxEPD_WHITE);
      printCenterString(display, "Connection failed", 100, 150);
      display->updateWindow(0, 0, GxEPD_WIDTH, GxEPD_HEIGHT);

      String wifi_ssid = preferences.getString("wifi_ssid", "");
      String wifi_passwd = preferences.getString("wifi_passwd", "");
      if (wifi_ssid != "") {
        WiFi.mode(WIFI_AP_STA);
        WiFi.begin(wifi_ssid.c_str(), wifi_passwd.c_str());
      }
    }

    done = true;
  }
}

void AppWiFiSmartconfig::exit() { preferences.end(); }

std::unique_ptr<AppWiFiSmartconfig> appWiFiSmartconfig(new AppWiFiSmartconfig("Connect to WiFi", icon_app_wifi_smartconfig));