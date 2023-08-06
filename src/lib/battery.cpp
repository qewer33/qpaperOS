#include "battery.h"

int calculateBatteryStatus() {
  int bat = 0;
  for (uint8_t i = 0; i < 25; i++) {
    bat += analogRead(BAT_ADC);
  }
  bat /= 25;
  float volt = (bat * 3.3 / 4096);
  return (int)(volt * 1000);
}