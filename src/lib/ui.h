#pragma once

#include "Arduino.h"
#include "GxEPD.h"
#include <GxDEPG0150BN/GxDEPG0150BN.h> // 1.54" b/w 200x200

void printLeftString(GxEPD_Class *display, const char *buf, int x, int y);
void printRightString(GxEPD_Class *display, const char *buf, int x, int y);
void printCenterString(GxEPD_Class *display, const char *buf, int x, int y);