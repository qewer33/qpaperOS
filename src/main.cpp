#include "AceButton.h"
#include "Adafruit_I2CDevice.h"
#include "ArduinoJson.h"
#include "ESP32Time.h"
#include "GxDEPG0150BN/GxDEPG0150BN.h" // 1.54" b/w 200x200
#include "GxEPD.h"
#include "GxIO/GxIO.h"
#include "GxIO/GxIO_SPI/GxIO_SPI.h"
#include "HTTPClient.h"
#include "Preferences.h"
#include "TinyGPSPlus.h"
#include "WiFi.h"
#include "soc/rtc_cntl_reg.h"
#include "soc/soc.h"
#include "time.h"

#include "home.h"
#include "lib/battery.h"
#include "lib/log.h"
#include "os_config.h"
#include "wakeup.h"

String wifi_ssid = "SUPERONLINE_WiFi_0657"; //"your ssid";
String wifi_passwd = "YLNXEKY9W9TR";        //"your password";

GxIO_Class io(SPI, /*CS*/ EPD_CS, /*DC=*/EPD_DC, /*RST=*/EPD_RESET);
GxEPD_Class display(io, /*RST=*/EPD_RESET, /*BUSY=*/EPD_BUSY);

ESP32Time rtc;
TinyGPSPlus gps;
HardwareSerial gpsPort(Serial2);
Preferences preferences;

RTC_DATA_ATTR WakeupFlag wakeup = WakeupFlag::WAKEUP_INIT;
RTC_DATA_ATTR uint32_t wakeupCount = 0;

uint32_t sleepTimer = 0;

float latitude = 0, longtitude = 0;
uint8_t satellites = 0;

void WiFiConnected(WiFiEvent_t event, WiFiEventInfo_t info) {
  log(LOG_INFO, "WiFi connected");
  configTime(GMT_OFFSET_SEC, DAY_LIGHT_OFFSET_SEC, NTP_SERVER1);
  log(LOG_INFO, "Time synchronized from WiFi");
}

hw_timer_t *uiTimer = NULL;
volatile SemaphoreHandle_t timerSemaphore;

void ARDUINO_ISR_ATTR onTimer() { xSemaphoreGiveFromISR(timerSemaphore, NULL); }

void setup() {

  Serial.begin(115200);
  delay(10);
  log(LOG_INFO, "Welcome to qPaperOS!");
  log(LOG_SUCCESS, "Serial communication initiliazed");

  gpsPort.begin(9600, SERIAL_8N1, GPS_RX, GPS_TX);
  SPI.begin(SPI_SCK, -1, SPI_DIN, EPD_CS);
  digitalWrite(GPS_RES, LOW);

  pinMode(PWR_EN, OUTPUT);
  digitalWrite(PWR_EN, HIGH);
  pinMode(PIN_KEY, INPUT_PULLUP);

  pinMode(BAT_ADC, ANALOG);
  adcAttachPin(BAT_ADC);
  analogReadResolution(12);
  analogSetWidth(50);
  calculateBatteryStatus();
  log(LOG_SUCCESS, "Hardware pins initiliazed");

  WiFi.onEvent(WiFiConnected, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_CONNECTED);

  timerSemaphore = xSemaphoreCreateBinary();
  uiTimer = timerBegin(0, 80, true);
  timerAttachInterrupt(uiTimer, &onTimer, true);
  timerAlarmWrite(uiTimer, 1000000, true);
  timerAlarmEnable(uiTimer);
  log(LOG_SUCCESS, "Hardware timer initiliazed");

  preferences.begin(PREFS_KEY);
  preferences.putString("wifi_ssid", wifi_ssid);
  preferences.putString("wifi_passwd", wifi_passwd);
  log(LOG_SUCCESS, "Preferences initiliazed");

  configTime(GMT_OFFSET_SEC, DAY_LIGHT_OFFSET_SEC, nullptr);
  log(LOG_SUCCESS, "Time configured");

  display.init();
  display.setRotation(1);
  log(LOG_SUCCESS, "Display initiliazed");

  if (digitalRead(PIN_KEY) == 0)
    wakeup = WakeupFlag::WAKEUP_FULL;
  log(LOG_INFO, "Starting wakeup process...");

  switch (wakeup) {
  case WakeupFlag::WAKEUP_INIT:
    wakeupInit(&wakeup, &wakeupCount, &display, &rtc, &preferences);
    break;
  case WakeupFlag::WAKEUP_FULL:
    wakeupFull(&wakeup, &wakeupCount, &display, &rtc, &preferences);
    break;
  case WakeupFlag::WAKEUP_LIGHT:
    wakeupLight(&wakeup, &wakeupCount, &display, &rtc, &preferences);
    break;
  }

  log(LOG_SUCCESS, "Wakeup process completed");
}

void loop() {
  if (xSemaphoreTake(timerSemaphore, 0) == pdTRUE)
    sleepTimer++;

  switch (wakeup) {
  case WakeupFlag::WAKEUP_INIT:
    if (sleepTimer == 60) {
      wakeup = WakeupFlag::WAKEUP_LIGHT;
      esp_sleep_enable_timer_wakeup(1000000);
      esp_deep_sleep_start();
    }
    break;
  case WakeupFlag::WAKEUP_FULL:
    if (xSemaphoreTake(timerSemaphore, 0) == pdTRUE) {
      drawHomeUI(&display, &rtc, calculateBatteryStatus());
      display.updateWindow(0, 0, GxEPD_WIDTH, GxEPD_HEIGHT, false);
    }

    if (sleepTimer == 30) {
      wakeup = WakeupFlag::WAKEUP_LIGHT;
      esp_sleep_enable_timer_wakeup(1000000);
      esp_deep_sleep_start();
    }
    break;
  case WakeupFlag::WAKEUP_LIGHT:
    if (sleepTimer == 15) {
      esp_sleep_enable_timer_wakeup(UPDATE_WAKEUP_TIMER_US - 15000000);
      esp_deep_sleep_start();
    }
    break;
  }

  delay(10);
}

/*
void gpsRead() {
  while (gpsPort.available()) {
    gps.encode(gpsPort.read());
  }
}
*/