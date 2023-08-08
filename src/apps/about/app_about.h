#pragma once

#include "apps.h"
#include "resources/app_icons.h"
#include "resources/fonts/Outfit_60011pt7b.h"

class AppAbout : public App {
public:
  using App::App;
  void drawUI(GxEPD_Class *display) override;
};

extern std::unique_ptr<AppAbout> appAbout;