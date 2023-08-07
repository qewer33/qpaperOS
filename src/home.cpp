#include "home.h"

const char *months[] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

const char *days[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

void drawHomeUI(GxEPD_Class *display, ESP32Time *rtc, int batteryStatus) {
  display->fillScreen(GxEPD_WHITE);
  display->setTextColor(GxEPD_BLACK);
  display->setTextWrap(false);

  // Time
  display->setFont(&Outfit_80036pt7b);
  String hoursFiller = rtc->getHour(true) < 10 ? "0" : "";
  String minutesFiller = rtc->getMinute() < 10 ? "0" : "";
  String timeStr = hoursFiller + String(rtc->getHour(true)) + ":" + minutesFiller + String(rtc->getMinute());
  printCenterString(display, timeStr.c_str(), 100, 125);

  // Date
  display->setFont(&Outfit_60011pt7b);
  printCenterString(display, String(String(days[rtc->getDayofWeek()]) + ", " + String(months[rtc->getMonth()]) + " " + String(rtc->getDay())).c_str(),
                    100, 60);

  // Battery
  display->setFont(&Outfit_60011pt7b);
  printRightString(display, String(String(batteryStatus) + "%").c_str(), 166, 22);

  const unsigned char *icon_battery_small_array[6] PROGMEM = {epd_bitmap_icon_battery_0_small,  epd_bitmap_icon_battery_20_small,
                                                              epd_bitmap_icon_battery_40_small, epd_bitmap_icon_battery_60_small,
                                                              epd_bitmap_icon_battery_80_small, epd_bitmap_icon_battery_100_small};

  display->drawBitmap(170, 2, icon_battery_small_array[batteryStatus / 20], 28, 28, GxEPD_BLACK);

  // Status icons
  display->drawBitmap(2, 2, icon_wifi_small, 28, 28, GxEPD_BLACK);
  display->drawBitmap(30, 2, icon_no_ble_small, 28, 28, GxEPD_BLACK);
  display->drawBitmap(58, 2, icon_gps_small, 28, 28, GxEPD_BLACK);

  // Prayer time
  display->drawBitmap(2, 142, icon_prayer_small, 28, 28, GxEPD_BLACK);
  printLeftString(display, "Isha, 22:10", 34, 164);

  // Steps
  display->drawBitmap(2, 170, icon_steps_small, 28, 28, GxEPD_BLACK);
  printLeftString(display, "7.546", 34, 192);

  // Weather
  display->drawBitmap(170, 170, icon_weather_small, 28, 28, GxEPD_BLACK);
  printRightString(display, "24°C", 166, 192);
}