#include "battery.h"

int calculateBatteryStatus() {
  int bat = 0;
  for (uint8_t i = 0; i < 25; i++) {
    bat += analogRead(BAT_ADC);
  }
  bat /= 25;
  float volt = (bat * 3.3 / 4096) * 2;
  volt = bat > 2500 ? 1 : volt / 4.2;
  return (int)constrain(map(volt * 100, 0, 86, 0, 100), 0, 100);
}