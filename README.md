![banner](assets/qpaper_banner.png)

qpaperOS is a smartwatch firmware implementation for the LILYGO T-Wrist E-Paper ESP32 development board. This repositoy houses the qpaperOS source code along with 3D models and the FreeCAD file of the smartwatch case to make your very own qpaper!

## The Product

![screenshot](assets/qpaper_screenshot.jpeg)

More photos of the complete watch can be found under `assets/photos/` (https://github.com/qewer33/qpaperOS/tree/main/assets/photos)

qpaper is meant to be a minimal and lighweight smartwatch that does what it needs to do: always display time. The low power E-Paper screen of the qpaper allows it to always display the time and other information without having the display blank. The ESP32 wakes up every minute to update the display to show the correct time. More information about how the firmware works can be found in the next section.

qpaper has a battery life of approx. ~5 days with a 250 mAh battery.

### Assembly

The outer sheell/case of the qpaper is meant to be 3D printed and assembled with 4 short M2 screws. The 3D models and FreeCAD file for the case can be found under `assets/3d/` (https://github.com/qewer33/qpaperOS/tree/main/assets/3d)

Required parts for assembly:
- LILYGO T-Wrist E-Paper ESP32 development board
    - https://www.lilygo.cc/products/t-wrist-e-paper-1-54-inch-display?variant=42405749948597 (Note: you don't need to buy the GPS version if you don't want the GPS features but if you don't have a LILYGO T-U2T, buy a version with T-U2T bundled. You *need* T-U2T to program the board!)
- 3D printed "Shell" piece
- 3D printed "Bottom Plate" piece
- 3D printed "Buttons" piece
- 250 mAh 1S lipo battery
- Half-size uFl GPS antenna (not required for the non-GPS version of the development board)
- 4x short M2/M1.6/M1.4 screws
- 22mm watch strap (compatible with Huawei GT2/GT3 straps)

## The Firmware

> [!WARNING]  
> qpaperOS is not yet finished, the information here might be inaccurate

qpaperOS is the firmware part of the qpaper project. It is developed to work with the LILYGO T-Wrist E-Paper ESP32 development board. It uses the espressif-esp32-arduino framework and PlatformIO for development.

Below are features that are implemented or planned:

- [x] Display time and date
- [x] Display battery percentage
- [x] Have the ESP32 MCU on deep sleep and wakeup every minute to update the display for power saving
- [x] Apps and app system (partially implemented)
- [ ] Themes and theme system (switchable watchface system)
- [ ] GPS functionality
- [ ] GPS based step counter
- [ ] Weather display
- [ ] Islamic prayer times display

### Apps

Apps live under the `src/apps` directory. Each app has it's own subdirectory and consists of 3 (or 2, one of them is optional) files.

```sh
src/
    apps/
        appname/
            app_appname.cpp
            app_appname.h
            app_appname_res.h  # optional
```

The `app_appname.h` file is the header file of the app. It contains the class and the extern instance definition of the app. Apps should extend the `App` class defined in `src/apps.h`. The methods that are going to be used by the app should be overriden methods from the `App` class. The instance should be an instance of the newly defined app class wrapped in an `std::unique_pointer`. There are 5 methods that apps can override from the App class:

- `setup()`: Runs before the app gets started. Useful for initializing variable defaults or loading preferences.
- `drawUI(GxEPD_Class *display)`: Runs every frame when the app is running. This method should draw the user interface of the app using `display`.
- `exit()`: Runs when the app gets exited. Useful for saving preferences and such.
- `buttonClick()`: Runs when the user button gets clicked while in the app.
- `buttonDoubleClick()`: Runs when the user button gets double clicked while in the app.

The `app_appname.cpp` file is the source file of the app. The source file should define the instance and implement the necessary app methods. The app constructor takes 2 argumnets: the first argument `String name` is the name and the second argument `uint16_t* icon` is the icon resource of the app.

The `app_appname_res.h` file contains the custom resources that are used by the app. These resources can be fonts, icons etc. This file is not necessary if the app doesn't have any custom resources. The app icon should go in `src/resources/app_icons.h`, not the app resource file.

The finished app should be included in `src/apps.cpp` and should be added to the apps array in the `initApps()` function.