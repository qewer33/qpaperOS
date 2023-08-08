#include "app_gps_sync.h"

void AppGpsSync::drawUI(GxEPD_Class *display) {
  display->fillScreen(GxEPD_WHITE);
  display->updateWindow(0, 0, GxEPD_WIDTH, GxEPD_HEIGHT);
}

std::unique_ptr<AppGpsSync> appGpsSync(new AppGpsSync("GPS Sync", icon_app_gps_connect));