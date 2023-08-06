# Changelog

* Unreleased
* 1.9 (2021-06-01)
    * Split [LadderButtonCalibrator](examples/LadderButtonCalibrator)
      from [LadderButtons](examples/LadderButtons) for convenience.
    * Add preliminary support for ATtiny85 using
      [SpenceKonde/ATTinyCore](https://github.com/SpenceKonde/ATTinyCore).
    * Add [LadderButtonsTiny](examples/LadderButtonsTiny), a demo of attaching
      2 buttons to the `RESET/A0` pin of the ATtiny85 micrcontroller using
      a resistor ladder. This avoids wasting the RESET pin, freeing the other 5
      GPIO pins for other purposes.
    * Update docs and examples for `LadderButtonConfig` and
      `EncodedButtonConfig` to prevent the creation of the unused
      SystemButtonConfig.
        * Uses the 4-parameter `AceButton()` constructor to set the
          `buttonConfig` parameter explicitly to `nullptr`.
        * Saves 30 bytes of flash and 26 bytes of RAM on AVR (e.g. ATmega328,
          ATtiny83) processors.
    * Split off `examples/Encoded4To2Buttons` from
      `examples/Encoded8To3Buttons` to make the usage of the
      `Encoded4To2ButtonConfig` and `Encoded8To3ButtonConfig` classes more
      clear.
    * Add `ButtonConfigFast1`, `ButtonConfigFast2`, and `ButtonConfigFast3`.
        * Implementations of `ButtonConfig` which use the `digitalWriteFast`
          library.
        * Saves 100-400 bytes of flash on AVR processors.
        * See [README.md#DigitalWriteFast](README.md#DigitalWriteFast).
* 1.8.3 (2021-04-18)
    * AceButton now has a
      [GitHub Discussion](https://github.com/bxparks/AceButton/discussions).
        * Use that for general questions and discussions.
        * Reserve [GitHub Issues](https://github.com/bxparks/AceButton/issues)
          for bugs and feature requests.
    * Add [Rate Limit](docs/resistor_ladder/README.md#RateLimitCheckButtons)
      warning on `LadderButtonsConfig::checkButtons()` should be limited to
      about 200 samples/second. This reduces the overhead of the slow
      `analogRead()` function, and avoids triggering a bug that disconnects the
      WiFi on the ESP8266.
    * Add general [Sampling Rate](README.md##SamplingRate) recommendation to
      sample at about 5 ms intervals, or 200 samples/second. Add code sample
      using non-blocking delay.
    * Update Makefiles used by EpoxyDuino for better compatibility with FreeBSD.
    * Update ESP32 Arduino Core from v1.0.4 to v1.0.6.
* 1.8.2 (2021-01-22)
    * Update UnixHostDuino 0.4 to EpoxyDuino 0.5.
    * No functional change in this release.
* 1.8.1 (2021-01-18)
    * Add official support for STM32 by validating on a "Blue Pill" board.
    * Add scripts to automate collection of `AutoBenchmark` data. Add
      benchmarks for STM32 microcontroller.
    * No functional change in this release.
* 1.8 (2020-11-21)
    * Add `kEventLongReleased` event type which is triggered after a
      `kEventLongPressed` as a substitute for `kEventReleased` when
      `kFeatureSuppressAfterLongPress` is used to suppress the `kEventReleased`
      event. See the new [Distinguishing Pressed and
      LongPressed](README.md#PressedAndLongPressed) section in the README.md and
      the new [examples/PressedAndLongPressed](examples/PressVersusLongPress)
      sample code.
    * **Potentially Breaking**: Completely remove
      `src/ace_button/AdjustableButtonConfig.h`. It was deprecated over 2 years
      ago in v1.3.
* 1.7.1 (2020-11-12)
    * Add Table of Contents to README.md.
    * Regenerate MemoryBenchmark numbers for v1.7.
    * No functional change in this release.
* 1.7 (2020-10-31)
    * **Potentially Breaking**: Remove `src/ace_button/testing/TimingStats.h`
      and replace it with `TimingStats` from AceCommon library
      https://github.com/bxparks/AceCommon. Affects only
      `examples/AutoBenchark/AutoBenchmark.ino`.
* 1.6.1 (2020-09-27)
    * Expose the `ButtonConfig` virtual destructor for all architectures
      except the AVR. In other words, instead of whitelisting only ESP8266 and
      ESP32, we blacklist just the AVR chips. This will allow most
      microcontrollers with sufficient memory to create AceButton objects on the
      heap without triggering compiler warnings. Most AVR chips have limited
      amount of flash and static memory, and pulling in the virtual destructor
      causes the flash size of to increase by 600 bytes, which is too large.
* 1.6 (2020-09-18)
    * Remove 'class Print' forward declaration that breaks megaAVR like Nano
      Every.
    * Mark non-virtual const functions with `const`, which preserves backwards
      compatibility.
    * Add support for object-based event handler using `IEventHandler` and
      `ButtonConfig::setIEventHanlder()`. Fixes [Issue
      #50](https://github.com/bxparks/AceButton/issues/50). Regenerate library
      code size numbers, showing increase of 34-64 bytes of flash memory.
    * Add examples of how to configure 2 AceButtons using either a single
      ButtonConfig or 2 ButtonConfigs.
    * **Potentially Breaking**: Change `ButtonConfig::getEventHandler()` to be
      an *internal* and *deprecated* method. Replaced with
      `ButtConfig::dispatchEvent()` which is also an internal method.
* 1.5 (2020-06-27)
    * Add `LadderButtonConfig` class to support multiple buttons on a single
      analog pin using `analogRead()`. Add documentation in
      [docs/resistor_ladder](docs/resistor_ladder).
      Fixes [Issue #43](https://github.com/bxparks/AceButton/issues/43).
    * Add a virtual destructor for `ButtonConfig`, but *only* on the ESP8266 and
      ESP32, to allow polymorphic objects to be created and deleted on the heap.
      Fixes [Issue #46](https://github.com/bxparks/AceButton/issues/46). No
      virtual destructor on 8-bit processors because it causes the flash memory
      code size to increase by 600 bytes.
    * Use `ButtonConfig() = default` constructor implementation instead of
      explicit empty body, saving 40-80 bytes.
    * **Potential Breaking Change**: Remove `src/AdjustableButtonConfig.h` and
      `src/ButtonConfig.h` files which were deprecated 2 years ago, and
      contained nothing.
    * **Breaking Change**: Remove protected virtual method
      `ButtonConfig::init()`, replaced with public `resetFeatures()`, saving
      a few dozen bytes of flash.
    * **Breaking Change**: Remove `TimingStats` from `ButtonConfig` and
      `AceButton`. Remove `ButtonConfig::setTimingStats()` and
      `ButtonConfig::getTimingStats()`. Reduces the code size by 13% (~150 out
      of ~1150 bytes).
    * **Breaking Change**: Move `TimingStats.h` file into
      `src/ace_button/testing` and move into `ace_button::testing` namespace.
    * **Breaking Change**: Remove unused `ButtonConfig::getClockMicros()` saving
      a few dozen bytes.
    * All these changes result in reducing the code size by 350 bytes on a
      SparkFun Pro Micro for a real-world app with 2 AceButtons and 1
      ButtonConfig.
* 1.4.3 (2020-05-02)
    * Update README.md to disambiguate overloaded constructors for
      `AceButton(0)`
      [Issue #40](https://github.com/bxparks/AceButton/issues/40).
    * Add GitHub Actions to run unit tests using UnixHostDuino.
* 1.4.2 (2020-02-03)
    * Fix compiler warnings.
    * Fix broken automated tests.
* 1.4.1 (2020-01-20)
    * Implement generalized `EncodedButtonConfig` which can handle
      `M=2^N-1` buttons using `N` pins using an optimization that makes
      only `N` calls to `digitalRead()` instead of `M * N` calls. Add
      `examples/Encoded16To4Buttons` example; rename `examples/EncodedButtons`
      to `examples/Encoded8To3Buttons`.
* 1.4 (2020-01-12)
    * Add better explanation and examples of multiple buttons to the README.md.
    * Add `Encoded8To3ButtonConfig` and ` Encoded4To2ButtonConfig` classes to
      support [BinaryEncoding](docs/binary_encoding/). Verified to work using
      diodes or a 74LS148 chip. Inspired by [Issue
      #32](https://github.com/bxparks/AceButton/issues/32).
* 1.3.5 (2019-08-11)
    * Verify library works on SAMD21 (Arduino Zero compatible) boards.
    * Reenable examples/CapacitiveButton on ESP8266 and ESP32.
      Looks like the development branch of the
      [CapacitiveSensor](https://github.com/PaulStoffregen/CapacitiveSensor)
      library added support for ESP32 in Dec 2018 but has not been released.
* 1.3.4 (2019-07-06)
    * Update compile-time checks for `HIGH` and `LOW` for compatibility with
      new megaAVR (e.g. Nano Every) boards which use enums instead of `#define`
      macros. Change value of `kButtonStatusUnknown` to 127 to avoid conflict
      with `PinStatus` enums.
    * Remove `ACE_BUTTON_INLINE` macros, was not doing anything.
    * Add `examples/ArrayButtons` to show how to define and initialize an array
      of AceButton instances.
    * Fix fqbn of ESP8266 after updating to ESP8266/Arduino Core 2.5.0.
* 1.3.3 (2019-03-10)
    * Add blurb about using `pinMode()` and button wiring configurations in
      README.md based on feedback from
      [Issue #19](https://github.com/bxparks/AceButton/issues/19).
    * Add `AceButton::isPressedRaw()` to determine the state of the button upon
      booting the device.
      (See [Issue #21](https://github.com/bxparks/AceButton/issues/21))
* 1.3.2 (2018-12-30)
    * Year-end maintenance release. No functional change.
    * Fix minor spelling and grammar mistakes in README.md.
    * Remove `virtual` when using `override` per C++ core style guide.
    * Update auniter.ini for compatibility with latest ESP32 and Sparkfun cores.
* 1.3.1 (2018-09-30)
    * Fix botched release on GitHub. Same as v1.3.
* 1.3 (2018-09-30)
    * Merge `AdjustableButtonConfig` into `ButtonConfig` and deprecated
      `AdjustableButtonConfig`. See
      [Issue #13](https://github.com/bxparks/AceButton/issues/13) for
      benchmarks which show that the theoretical increase of static RAM
      consumption does not often happen in practice because of compiler
      optimization.
    * Reduce default value of `getDebounceDelay()` from 50 ms to 20 ms
      to improve perceived responsiveness of buttons when they are rapidly
      pressed on and off. See
      [Issue #14](https://github.com/bxparks/AceButton/issues/14)
      for details.
    * Update `tests/auniter.ini` and `Jenkinsfile` for compatibility with
      AUniter v1.7. Add `CapacitiveSensor` to the exclude list for
      `env:esp8266` and `env:esp32` because it doesn't compile under those
      environments.
    * Remove leading zero in `ACE_BUTTON_VERSION` because that I forgot that it
      means an octal number.
* 1.2 (2018-08-14)
    * Add `AceButton(ButtonConfig*)` constructor to support constructor
      dependency injection. Recommended over `setButtonConfig()`.
      Should be 100% backwards compatible.
    * Add examples/CapacitiveSwitch program to demonstrate integration
      with CapacitiveSensor library to support capacitive switches.
    * Add continuous integration using AUniter/Jenkins.
* 1.1.1 (2018-06-21)
    * Fix compiler warnings about unused parameters in eventHandler callback.
    * Move AutoBenchmark results into AutoBenchmark/README.md.
    * Update various links in AceButton/README.md.
    * No functional or API change.
* 1.1.0 (2018-05-03)
    * Change to MIT License.
    * Add instrumentation of AceButton.check() using TimingStats to measure
      performance. (Fixes #9)
    * Add `examples/AutoBenchmark` sketch to auto-generate benchmarks for
      various microcontrollers.
    * Verify that library and examples compile, and the tests pass for ESP32.
    * Add a third method for distinguishing Clicked from DoubleClicked using
      both techniques described in 1.0.6.
* 1.0.6 (2018-03-25)
    * Add `kFeatureSuppressClickBeforeDoubleClick` flag to suppress
      Clicked event before a DoubleClicked event, at the cost of slower
      response time of the Clicked event. Added 2 more examples to demonstrate 2
      methods to distinguish between a Clicked and DoubleClicked.
    * Publish [doxygen docs](https://bxparks.github.io/AceButton/html/)
      on GitHub Pages.
* 1.0.5 (2018-03-17)
    * Migrate unit tests to [AUnit](https://github.com/bxparks/AUnit).
    * Fix various typos in README.md.
* 1.0.4 (2018-03-07)
    * Support ESP8266.
        * Split `loop()` in `Stopwatch.ino` into inner and outer loops, to
          allow `loop()` to return periodically.
        * Perform manual testing, since ArduinoUnit does not work on ESP8266.
    * Optimize `check()` so that `checkOrphanedClick()` is called only when
      needed.
    * README.md: add  benchmark numbers for ESP8266, fix typos.
    * Fix various compiler warnings about unused variables.
* 1.0.3 (2018-02-13)
    * Make library work on Teensy LC and 3.2.
        * Fix `elapsedTime` expression that breaks on 32-bit processors
          (whose `int` is 4 bytes instead of 2).
* 1.0.2 (2018-02-07)
    * Add documentation and unit tests for `AdjustableButtonConfig`.
    * Reduce `orphanClickDelay` to 1X `getDoubleClickDelay()` instead of 10X.
* 1.0.1 (2018-02-03)
    * Fix typo in 'library.properties'.
* 1.0.0 (2018-02-03)
    * Initial public release.
