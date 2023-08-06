#include "wakeup.h"

// Wakeup

void wakeupInit(WakeupFlag *wakeupType, unsigned int *wakeupCount, GxEPD_Class *display, ESP32Time *rtc, Preferences *preferences) {
  log(LOG_INFO, "WAKEUP_INIT");

  // rtc->setTime(preferences->getLong64("prev_time_unix", 0) + 60);

  display->fillScreen(GxEPD_WHITE);
  display->update();
  delay(1000);
  drawHomeUI(display, rtc, calculateBatteryStatus());
  display->update();

  WiFi.mode(WIFI_STA);
  WiFi.begin(preferences->getString("wifi_ssid"), preferences->getString("wifi_passwd"));
  log(LOG_SUCCESS, "WiFi initiliazed");
}

void wakeupLight(WakeupFlag *wakeupType, unsigned int *wakeupCount, GxEPD_Class *display, ESP32Time *rtc, Preferences *preferences) {
  log(LOG_INFO, "WAKEUP_LIGHT");

  drawHomeUI(display, rtc, calculateBatteryStatus());
  display->update();
  display->powerDown();

  preferences->putLong64("prev_time_unix", rtc->getEpoch());

  wakeupCount++;

  /*
  if (*wakeupCount % 30 == 0) {
    WiFi.mode(WIFI_STA);
    WiFi.begin(preferences->getString("wifi_ssid"), preferences->getString("wifi_passwd"));
    return;
  }
  */

  log(LOG_INFO, "Going to sleep...");
  digitalWrite(PWR_EN, LOW);
  esp_sleep_enable_ext0_wakeup((gpio_num_t)PIN_KEY, 0);
  esp_sleep_enable_timer_wakeup(UPDATE_WAKEUP_TIMER_US);
  esp_deep_sleep_start();
}

void wakeupFull(WakeupFlag *wakeupType, unsigned int *wakeupCount, GxEPD_Class *display, ESP32Time *rtc, Preferences *preferences) {
  log(LOG_INFO, "WAKEUP_FULL");

  wakeupCount = 0;

  WiFi.mode(WIFI_STA);
  WiFi.begin(preferences->getString("wifi_ssid"), preferences->getString("wifi_passwd"));
}

// Loop

void wakeupInitLoop(WakeupFlag *wakeupType, unsigned int *wakeupCount, GxEPD_Class *display, ESP32Time *rtc, Preferences *preferences) {
  log(LOG_INFO, "WAKEUP_FULL");

  wakeupCount = 0;

  WiFi.mode(WIFI_STA);
  WiFi.begin(preferences->getString("wifi_ssid"), preferences->getString("wifi_passwd"));
}