#include "ui.h"

void printLeftString(GxEPD_Class *display, const char *buf, int x, int y) {
  display->setCursor(x, y);
  display->print(buf);
}

void printRightString(GxEPD_Class *display, const char *buf, int x, int y) {
  int16_t x1, y1;
  uint16_t w, h;
  display->getTextBounds(buf, x, y, &x1, &y1, &w, &h);
  display->setCursor(x - w, y);
  display->print(buf);
}

void printCenterString(GxEPD_Class *display, const char *buf, int x, int y) {
  int16_t x1, y1;
  uint16_t w, h;
  display->getTextBounds(buf, x, y, &x1, &y1, &w, &h);
  display->setCursor(x - w / 2, y);
  display->print(buf);
}