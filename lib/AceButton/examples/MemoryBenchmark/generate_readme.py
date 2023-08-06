#!/usr/bin/python3
#
# Python script that regenerates the README.md from the embedded template. Uses
# ./generate_table.awk to regenerate the ASCII tables from the various *.txt
# files.

from subprocess import check_output

attiny_results = check_output(
    "./generate_table.awk < attiny.txt", shell=True, text=True)
nano_results = check_output(
    "./generate_table.awk < nano.txt", shell=True, text=True)
micro_results = check_output(
    "./generate_table.awk < micro.txt", shell=True, text=True)
samd_results = check_output(
    "./generate_table.awk < samd.txt", shell=True, text=True)
esp8266_results = check_output(
    "./generate_table.awk < esp8266.txt", shell=True, text=True)
esp32_results = check_output(
    "./generate_table.awk < esp32.txt", shell=True, text=True)
teensy32_results = check_output(
    "./generate_table.awk < teensy32.txt", shell=True, text=True)
stm32_results = check_output(
    "./generate_table.awk < stm32.txt", shell=True, text=True)

print(f"""\
# Memory Benchmark

The `MemoryBenchmark.ino` compiles example code snippets using the various
`ButtonConfig` classes. The `FEATURE` macro flag controls which feature is
compiled. The `collect.sh` edits this `FEATURE` flag programmatically, then runs
the Arduino IDE compiler on the program, and extracts the flash and static
memory usage into a text file (e.g. `nano.txt`).

The numbers shown below should be considered to be rough estimates. It is often
difficult to separate out the code size of the library from the overhead imposed
by the runtime environment of the processor. For example, it often seems like
the ESP8266 allocates flash memory in blocks of a certain quantity, so the
calculated flash size can jump around in unexpected ways.

**Version**: AceButton v1.9

**DO NOT EDIT**: This file was auto-generated using `make README.md`.

## How to Generate

This requires the [AUniter](https://github.com/bxparks/AUniter) script
to execute the Arduino IDE programmatically.

The `Makefile` has rules for several microcontrollers:

```
$ make benchmarks
```
produces the following files:

```
nano.txt
micro.txt
samd.txt
esp8266.txt
esp32.txt
teensy32.txt
```

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

## Functionality

* Baseline: A program that does (almost) nothing
* ButtonConfig: 1 `AceButton` using the default system `ButtonConfig`
* Encoded4To2ButtonConfig: 3 `AceButton` using 1 `Encoded4To2ButtonConfig`
* Encoded8To3ButtonConfig: 7 `AceButton` using 1 `Encoded8To3ButtonConfig`
* EncodedButtonConfig: 7 `AceButton` using 1 `EncodedButtonConfig`
* LadderButtonConfig: 7 `AceButton` using 1 `LadderButtonConfig`

## Library Size Changes

Prior to v1.5, the 'Baseline' numbers also included the system `ButtonConfig`
that is automatically created by the library. But in v1.5, I discovered that
defining the `ButtonConfig::ButtonConfig()` constructor with a `= default`
instead of explicitly defining an empty body `{{}}` allows the compiler/linker
to completely remove the static instance of the system `ButtonConfig` if it is
never referenced. This caused the incremental size of the library as listed in
the tables to increase by 200-1000 bytes, but the total flash size of any
program that uses the AceButton library to actually decreased by 40-80 bytes
because it could use the `= default` implementation instead of an explicit empty
implementation.

In v1.6, adding support for using `IEventHandler` as the event handler increased
the library flash size between 34 to 64 bytes on 8-bit and 32-bit processors.
The RAM size is unchanged because only a single bit-field flag is used which
required no additional fields to be added to the `ButtonConfig`.

In v1.6.1, making the destructor in `ButtonConfig` to be `virtual` was extended
to SAMD and Teensy architectures (joining ESP8266 and ESP32). This seems to
cause flash memory usage to increase by about 200-300 bytes on the SAMD, and
about 3500 bytes on the Teensy 3.2. Even though those platforms have significant
amounts of flash memory (256kB if I recall), those numbers are not ideal. The
mitigating factor is that the increase in flash size is probably due `malloc()`
and `free()` so if any other library brings those in (e.g. the `String` class),
then the incremental cost is not significant.

In v1.8, adding support for `kEventLongReleased` increased the flash memory
consumption by 8 to 56 bytes.

In v1.8.3, I upgraded my ESP32 Arduino Core from v1.0.4 to v1.0.6. For some
reason, it causes the `LadderButtConfig` to increase flash consumption from ~5kB
to ~19kB. The CPU time consumption for `LadderButtonConfig` also went up by a
factor of 2.5X (24 micros to 67 micros). It looks like they changed the
implementation of `analogRead()` in v1.0.5. See
https://github.com/espressif/arduino-esp32/issues/4973 and
https://github.com/espressif/arduino-esp32/pull/3377.

In v1.9, added `ButtonConfigFast1`, `ButtonConfigFast2`, and `ButtonConfigFast3`
to support `digitalWriteFast.h` libraries. Reduces flash consumption on AVR
processors by 100 to 400 bytes. Added preliminary support for ATtiny85 using
[SpenceKonde/ATTinyCore](https://github.com/SpenceKonde/ATTinyCore).

## ATtiny85

* 8MHz ATtiny85
* Arduino IDE 1.8.13
* SpenceKonde/ATTinyCore 1.5.2

```
{attiny_results}
```

## Arduino Nano

* 16MHz ATmega328P
* Arduino IDE 1.8.13
* Arduino AVR Boards 1.8.3

```
{nano_results}
```

## SparkFun Pro Micro

* 16 MHz ATmega32U4
* Arduino IDE 1.8.13
* SparkFun AVR Boards 1.1.13

```
{micro_results}
```

## SAMD21 M0 Mini

* 48 MHz ARM Cortex-M0+
* Arduino IDE 1.8.13
* SparkFun SAMD Core 1.8.1

```
{samd_results}
```

## ESP8266

* NodeMCU 1.0 clone, 80MHz ESP8266
* Arduino IDE 1.8.13
* ESP8266 Boards 2.7.4

```
{esp8266_results}
```

## STM32

* STM32 "Blue Pill", STM32F103C8, 72 MHz ARM Cortex-M3
* Arduino IDE 1.8.13
* STM32duino 1.9.0

```
{stm32_results}
```

## ESP32

* ESP32-01 Dev Board, 240 MHz Tensilica LX6
* Arduino IDE 1.8.13
* ESP32 Boards 1.0.6

```
{esp32_results}
```

## Teensy 3.2

* 96 MHz ARM Cortex-M4
* Arduino IDE 1.8.13
* Teensyduino 1.53
* Compiler options: "Faster"

```
{teensy32_results}
```
""")
