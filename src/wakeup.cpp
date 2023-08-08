#include "wakeup.h"

// Setup

void wakeupInit(WakeupFlag *wakeupType, unsigned int *wakeupCount, GxEPD_Class *display, ESP32Time *rtc, Preferences *preferences) {
  log(LogLevel::INFO, "WAKEUP_INIT");

  rtc->setTime(preferences->getLong64("prev_time_unix", 0) + 15);

  display->fillScreen(GxEPD_WHITE);
  display->update();
  delay(1000);
  drawHomeUI(display, rtc, calculateBatteryStatus());
  display->update();

  WiFi.mode(WIFI_STA);
  WiFi.begin(preferences->getString("wifi_ssid"), preferences->getString("wifi_passwd"));
  log(LogLevel::SUCCESS, "WiFi initiliazed");
}

void wakeupLight(WakeupFlag *wakeupType, unsigned int *wakeupCount, GxEPD_Class *display, ESP32Time *rtc, Preferences *preferences) {
  log(LogLevel::INFO, "WAKEUP_LIGHT");
  setCpuFrequencyMhz(80);

  drawHomeUI(display, rtc, calculateBatteryStatus());
  display->update();
  display->powerDown();

  preferences->putLong64("prev_time_unix", rtc->getEpoch());

  wakeupCount++;

  if (*wakeupCount % 30 == 0) {
    WiFi.mode(WIFI_STA);
    WiFi.begin(preferences->getString("wifi_ssid"), preferences->getString("wifi_passwd"));
    return;
  }

  log(LogLevel::INFO, "Going to sleep...");
  digitalWrite(PWR_EN, LOW);
  esp_sleep_enable_ext0_wakeup((gpio_num_t)PIN_KEY, 0);
  esp_sleep_enable_timer_wakeup(UPDATE_WAKEUP_TIMER_US);
  esp_deep_sleep_start();
}

void wakeupFull(WakeupFlag *wakeupType, unsigned int *wakeupCount, GxEPD_Class *display, ESP32Time *rtc, Preferences *preferences) {
  log(LogLevel::INFO, "WAKEUP_FULL");
  setCpuFrequencyMhz(240);

  wakeupCount = 0;

  initApps();
  log(LogLevel::SUCCESS, "Apps initiliazed");

  WiFi.mode(WIFI_STA);
  WiFi.begin(preferences->getString("wifi_ssid"), preferences->getString("wifi_passwd"));

  display->fillScreen(GxEPD_WHITE);
  display->updateWindow(0, 0, GxEPD_WIDTH, GxEPD_HEIGHT);
}

// Loop

void wakeupInitLoop(WakeupFlag *wakeupType, unsigned int sleepTimer, GxEPD_Class *display, ESP32Time *rtc) {
  if (sleepTimer == 30) {
    *wakeupType = WakeupFlag::WAKEUP_LIGHT;
    esp_sleep_enable_timer_wakeup(1000000);
    esp_deep_sleep_start();
  }
}

void wakeupLightLoop(WakeupFlag *wakeupType, unsigned int sleepTimer, GxEPD_Class *display, ESP32Time *rtc) {
  if (sleepTimer == 15) {
    esp_sleep_enable_timer_wakeup(UPDATE_WAKEUP_TIMER_US - 15000000);
    esp_deep_sleep_start();
  }
}

void wakeupFullLoop(WakeupFlag *wakeupType, unsigned int sleepTimer, GxEPD_Class *display, ESP32Time *rtc, AwakeState awakeState) {
  if (awakeState == AwakeState::APPS_MENU) {
    drawAppsListUI(display, rtc, calculateBatteryStatus());
    display->updateWindow(0, 0, GxEPD_WIDTH, GxEPD_HEIGHT);
  } else {
    apps[currentAppIndex]->drawUI(display);
  }

  if (sleepTimer == 15) {
    *wakeupType = WakeupFlag::WAKEUP_LIGHT;
    esp_sleep_enable_timer_wakeup(1000000);
    esp_deep_sleep_start();
  }
}