# AutoBenchmark

This sketch measures the amount of time consumed by the `AceButton::check()`
method when processing various button events. It uses a special
`ProfilingButtonConfig` object that allows the program to inject button events
into the library. The profiling numbers come from activating the `TimingStats`
object that has been instrumented into the `AceButton::check()` method.

Note that `ProfilingButtonConfig` class generates synthetic button events,
bypassing the actual `digitalRead()` function. The `digitalRead()` function on
an Arduino AVR platform (UNO, Nano, etc) is
[known to be slow](https://forum.arduino.cc/index.php?topic=337578)
which will add to the timing values shown below in actual usage.
The [digitalWriteFast library](https://github.com/NicksonYap/digitalWriteFast)
might be an alternative if speed is critical.

**Version**: AceButton v1.9

**DO NOT EDIT**: This file was auto-generated using `make README.md`.

## Dependencies

This program depends on the following libraries:

* [AceCommon](https://github.com/bxparks/AceCommon)
* [AceButton](https://github.com/bxparks/AceButton)

## How to Generate

This requires the [AUniter](https://github.com/bxparks/AUniter) script
to execute the Arduino IDE programmatically.

The `Makefile` has rules to generate the `*.txt` results file for several
microcontrollers that I usually support, but the `$ make benchmarks` command
does not work very well because the USB port of the microcontroller is a
dynamically changing parameter. I created a semi-automated way of collect the
`*.txt` files:

1. Connect the microcontroller to the serial port. I usually do this through a
USB hub with individually controlled switch.
2. Type `$ auniter ports` to determine its `/dev/ttyXXX` port number (e.g.
`/dev/ttyUSB0` or `/dev/ttyACM0`).
3. If the port is `USB0` or `ACM0`, type `$ make nano.txt`, etc.
4. Switch off the old microcontroller.
5. Go to Step 1 and repeat for each microcontroller.

The `generate_table.awk` program reads one of `*.txt` files and prints out an
ASCII table that can be directly embedded into this README.md file. For example
the following command produces the table in the Nano section below:

```
$ ./generate_table.awk < nano.txt
```

Fortunately, we no longer need to run `generate_table.awk` for each `*.txt`
file. The process has been automated using the `generate_readme.py` script which
will be invoked by the following command:
```
$ make README.md
```

The CPU times below are given in microseconds. The "samples" column is the
number of `TimingStats::update()` calls that were made.

## CPU Time Changes

**v1.8.1**
* Add benchmarks for STM32.

**v1.8.3**
* Upgrade the ESP32 Arduino Core from v1.0.4 to v1.0.6.
    * Most things remain the same, maybe slightly faster, except for
      `LadderButtonConfig`. The flash memory consumption goes up by about 14kB,
      and the CPU time for `LadderButtonConfig::checkButtons()` goes up
      significantly, from 24 micros on average to 67 micros. It looks like they
      changed the implementation of `analogRead()` in v1.0.5. See
      https://github.com/espressif/arduino-esp32/issues/4973 and
      https://github.com/espressif/arduino-esp32/pull/3377.

**v1.9**
* Add benchmarks for `ButtonConfigFast1<>`, `ButtonConfigFast2<>`, and
  `ButtonConfigFast3<>`, using one of the `<digitalWriteFast.h>` libraries for
  AVR processors.
    * The primary motivation of using `<digitalWriteFast.h>` is the 100-500 byte      reduction in flash memory consumption.
    * As a bonus, the `AceButton::check()` call becomes faster. Comparing the
      numbers for `Encoded4To2ButtonConfig` and `ButtonConfigFast3`, both
      perform 3 calls to `AceButton::check()`, but `ButtonConfigFast3` makes
      those calls almost twice as fast as `Encoded4To2ButtonConfig` (38
      microseconds versus 72 microseconds).

## Results

The following table shows the minimum, average, and maximum (min/avg/max)
time taken for a number of scenarios in units of microseconds:

* idle
    * one `AceButton::check()` call with no button events
* press/release
    * one `AceButton::check()` call with Press and Release events
* click
    * one `AceButton::check()` call with a Click event
* double_click
    * one `AceButton::check()` call with a DoubleClick event
* long_press/repeat_press
    * one `AceButton::check()` call with LongPress and RepeatPress events
* ButtonConfigFast1 (AVR-only)
    * 1 x `AceButton::check()` calls using `ButtonConfigFast1<>` with no events
* ButtonConfigFast2 (AVR-only)
    * 2 x `AceButton::check()` calls using `ButtonConfigFast2<>` with no events
* ButtonConfigFast3 (AVR-only)
    * 3 x `AceButton::check()` calls using `ButtonConfigFast3<>` with no events
* Encoded4To2ButtonConfig
    * 3 x `AceButton::check()` calls using `Encoded4To2ButtonConfig` with no
      events
* Encoded8To3ButtonConfig
    * 7 x `AceButton::check()` calls using `Encoded8To3ButtonConfig` with no
      events
* EncodedButtonConfig
    * one `EncodedButtonConfig::checkButtons()` call to support 7 virtual
      buttons, with no events
* LadderButtonConfig
    * one `LadderButtonConfig::checkButtons()` call to support 7 virtual
      buttons, with no events

### Arduino Nano

* 16MHz ATmega328P
* Arduino IDE 1.8.13
* Arduino AVR Boards 1.8.3
* `micros()` has a resolution of 4 microseconds

```
Sizes of Objects:
sizeof(AceButton): 14
sizeof(ButtonConfig): 18
sizeof(ButtonConfigFast1<>): 18
sizeof(ButtonConfigFast2<>): 18
sizeof(ButtonConfigFast3<>): 18
sizeof(Encoded4To2ButtonConfig): 21
sizeof(Encoded8To3ButtonConfig): 22
sizeof(EncodedButtonConfig): 25
sizeof(LadderButtonConfig): 26

CPU:
+---------------------------+-------------+---------+
| Button Event              | min/avg/max | samples |
|---------------------------+-------------+---------|
| idle                      |   8/ 15/ 24 |    1931 |
| press/release             |   8/ 16/ 24 |    1924 |
| click                     |  12/ 15/ 24 |    1924 |
| double_click              |  12/ 15/ 32 |    1921 |
| long_press/repeat_press   |  12/ 16/ 28 |    1924 |
|---------------------------+-------------+---------|
| ButtonConfigFast1         |   8/ 14/ 24 |    1933 |
| ButtonConfigFast2         |  20/ 27/ 36 |    1910 |
| ButtonConfigFast3         |  28/ 40/ 48 |    1886 |
|---------------------------+-------------+---------|
| Encoded4To2ButtonConfig   |  60/ 67/ 92 |    1839 |
| Encoded8To3ButtonConfig   | 164/186/208 |    1659 |
| EncodedButtonConfig       |  80/100/124 |    1785 |
| LadderButtonConfig        | 176/199/272 |    1640 |
+---------------------------+-------------+---------+

```

### SparkFun Pro Micro

* 16 MHz ATmega32U4
* Arduino IDE 1.8.13
* SparkFun AVR Boards 1.1.13
* `micros()` has a resolution of 4 microseconds

```
Sizes of Objects:
sizeof(AceButton): 14
sizeof(ButtonConfig): 18
sizeof(ButtonConfigFast1<>): 18
sizeof(ButtonConfigFast2<>): 18
sizeof(ButtonConfigFast3<>): 18
sizeof(Encoded4To2ButtonConfig): 21
sizeof(Encoded8To3ButtonConfig): 22
sizeof(EncodedButtonConfig): 25
sizeof(LadderButtonConfig): 26

CPU:
+---------------------------+-------------+---------+
| Button Event              | min/avg/max | samples |
|---------------------------+-------------+---------|
| idle                      |   8/ 15/ 20 |    1932 |
| press/release             |   8/ 15/ 24 |    1924 |
| click                     |   8/ 15/ 24 |    1924 |
| double_click              |  12/ 15/ 36 |    1922 |
| long_press/repeat_press   |  12/ 16/ 24 |    1924 |
|---------------------------+-------------+---------|
| ButtonConfigFast1         |   8/ 13/ 36 |    1936 |
| ButtonConfigFast2         |  20/ 26/ 48 |    1912 |
| ButtonConfigFast3         |  28/ 38/ 64 |    1890 |
|---------------------------+-------------+---------|
| Encoded4To2ButtonConfig   |  60/ 72/ 84 |    1830 |
| Encoded8To3ButtonConfig   | 172/195/216 |    1646 |
| EncodedButtonConfig       |  80/102/124 |    1782 |
| LadderButtonConfig        | 172/201/276 |    1635 |
+---------------------------+-------------+---------+

```

### SAMD21 M0 Mini

* 48 MHz ARM Cortex-M0+
* Arduino IDE 1.8.13
* SparkFun SAMD Core 1.8.1

```
Sizes of Objects:
sizeof(AceButton): 16
sizeof(ButtonConfig): 24
sizeof(Encoded4To2ButtonConfig): 28
sizeof(Encoded8To3ButtonConfig): 28
sizeof(EncodedButtonConfig): 36
sizeof(LadderButtonConfig): 36

CPU:
+---------------------------+-------------+---------+
| Button Event              | min/avg/max | samples |
|---------------------------+-------------+---------|
| idle                      |   5/  8/  9 |    2002 |
| press/release             |   5/  9/ 14 |    2002 |
| click                     |   4/  8/ 14 |    2002 |
| double_click              |   4/  8/ 16 |    2002 |
| long_press/repeat_press   |   5/  9/ 13 |    2002 |
|---------------------------+-------------+---------|
| Encoded4To2ButtonConfig   |  19/ 30/ 31 |    2002 |
| Encoded8To3ButtonConfig   |  49/ 76/ 80 |    2002 |
| EncodedButtonConfig       |  26/ 54/ 59 |    2002 |
| LadderButtonConfig        | 447/473/474 |    2002 |
+---------------------------+-------------+---------+

```

### STM32

* STM32 "Blue Pill", STM32F103C8, 72 MHz ARM Cortex-M3
* Arduino IDE 1.8.13
* STM32duino 1.9.0

```
Sizes of Objects:
sizeof(AceButton): 16
sizeof(ButtonConfig): 24
sizeof(Encoded4To2ButtonConfig): 28
sizeof(Encoded8To3ButtonConfig): 28
sizeof(EncodedButtonConfig): 36
sizeof(LadderButtonConfig): 36

CPU:
+---------------------------+-------------+---------+
| Button Event              | min/avg/max | samples |
|---------------------------+-------------+---------|
| idle                      |   3/  5/ 10 |    2002 |
| press/release             |   3/  5/  8 |    2002 |
| click                     |   3/  5/  8 |    2002 |
| double_click              |   3/  5/  9 |    2002 |
| long_press/repeat_press   |   3/  5/  7 |    2002 |
|---------------------------+-------------+---------|
| Encoded4To2ButtonConfig   |  14/ 20/ 21 |    2002 |
| Encoded8To3ButtonConfig   |  38/ 54/ 58 |    2002 |
| EncodedButtonConfig       |  19/ 33/ 35 |    2002 |
| LadderButtonConfig        |  82/ 96/ 97 |    2002 |
+---------------------------+-------------+---------+

```

### ESP8266

* NodeMCU 1.0 clone, 80MHz ESP8266
* Arduino IDE 1.8.13
* ESP8266 Boards 2.7.4

```
Sizes of Objects:
sizeof(AceButton): 16
sizeof(ButtonConfig): 24
sizeof(Encoded4To2ButtonConfig): 28
sizeof(Encoded8To3ButtonConfig): 28
sizeof(EncodedButtonConfig): 36
sizeof(LadderButtonConfig): 36

CPU:
+---------------------------+-------------+---------+
| Button Event              | min/avg/max | samples |
|---------------------------+-------------+---------|
| idle                      |   6/  8/ 52 |    1917 |
| press/release             |   6/  8/ 62 |    1907 |
| click                     |   6/  8/ 38 |    1902 |
| double_click              |   6/  8/ 32 |    1907 |
| long_press/repeat_press   |   6/  8/ 41 |    1903 |
|---------------------------+-------------+---------|
| Encoded4To2ButtonConfig   |  22/ 27/ 55 |    1878 |
| Encoded8To3ButtonConfig   |  54/ 66/ 90 |    1806 |
| EncodedButtonConfig       |  42/ 54/ 81 |    1831 |
| LadderButtonConfig        | 136/149/491 |    1682 |
+---------------------------+-------------+---------+

```

### ESP32

* ESP32-01 Dev Board, 240 MHz Tensilica LX6
* Arduino IDE 1.8.13
* ESP32 Boards 1.0.4

```
Sizes of Objects:
sizeof(AceButton): 16
sizeof(ButtonConfig): 24
sizeof(Encoded4To2ButtonConfig): 28
sizeof(Encoded8To3ButtonConfig): 28
sizeof(EncodedButtonConfig): 36
sizeof(LadderButtonConfig): 36

CPU:
+---------------------------+-------------+---------+
| Button Event              | min/avg/max | samples |
|---------------------------+-------------+---------|
| idle                      |   2/  3/ 16 |    2002 |
| press/release             |   2/  2/ 13 |    2002 |
| click                     |   2/  2/  8 |    2002 |
| double_click              |   2/  2/  5 |    2002 |
| long_press/repeat_press   |   2/  2/  4 |    2002 |
|---------------------------+-------------+---------|
| Encoded4To2ButtonConfig   |   7/  7/ 12 |    2002 |
| Encoded8To3ButtonConfig   |  15/ 18/ 19 |    2002 |
| EncodedButtonConfig       |  12/ 16/ 25 |    2002 |
| LadderButtonConfig        |  64/ 68/264 |    2002 |
+---------------------------+-------------+---------+

```

### Teensy 3.2

* 96 MHz ARM Cortex-M4
* Arduino IDE 1.8.13
* Teensyduino 1.53
* Compiler options: "Faster"

```
Sizes of Objects:
sizeof(AceButton): 16
sizeof(ButtonConfig): 24
sizeof(Encoded4To2ButtonConfig): 28
sizeof(Encoded8To3ButtonConfig): 28
sizeof(EncodedButtonConfig): 36
sizeof(LadderButtonConfig): 36

CPU:
+---------------------------+-------------+---------+
| Button Event              | min/avg/max | samples |
|---------------------------+-------------+---------|
| idle                      |   2/  3/  6 |    1988 |
| press/release             |   2/  4/  6 |    1987 |
| click                     |   2/  3/  6 |    1987 |
| double_click              |   1/  3/  8 |    1986 |
| long_press/repeat_press   |   2/  4/  6 |    1987 |
|---------------------------+-------------+---------|
| Encoded4To2ButtonConfig   |   7/ 12/ 15 |    1970 |
| Encoded8To3ButtonConfig   |  17/ 30/ 33 |    1937 |
| EncodedButtonConfig       |   8/ 22/ 25 |    1951 |
| LadderButtonConfig        |  16/ 28/ 36 |    1941 |
+---------------------------+-------------+---------+

```

