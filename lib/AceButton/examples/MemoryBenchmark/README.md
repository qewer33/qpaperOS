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
instead of explicitly defining an empty body `{}` allows the compiler/linker
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
+--------------------------------------------------------------+
| functionality                   |  flash/  ram |       delta |
|---------------------------------+--------------+-------------|
| Baseline                        |    406/   11 |     0/    0 |
|---------------------------------+--------------+-------------|
| ButtonConfig                    |   1554/   51 |  1148/   40 |
| ButtonConfigFast1               |   1450/   51 |  1044/   40 |
| ButtonConfigFast2               |   1408/   65 |  1002/   54 |
| ButtonConfigFast3               |   1454/   79 |  1048/   68 |
| Encoded4To2ButtonConfig         |   1812/   82 |  1406/   71 |
| Encoded8To3ButtonConfig         |   2070/  139 |  1664/  128 |
| EncodedButtonConfig             |   2118/  162 |  1712/  151 |
| LadderButtonConfig              |   2042/  175 |  1636/  164 |
+--------------------------------------------------------------+

```

## Arduino Nano

* 16MHz ATmega328P
* Arduino IDE 1.8.13
* Arduino AVR Boards 1.8.3

```
+--------------------------------------------------------------+
| functionality                   |  flash/  ram |       delta |
|---------------------------------+--------------+-------------|
| Baseline                        |    610/   11 |     0/    0 |
|---------------------------------+--------------+-------------|
| ButtonConfig                    |   1946/   51 |  1336/   40 |
| ButtonConfigFast1               |   1662/   51 |  1052/   40 |
| ButtonConfigFast2               |   1630/   65 |  1020/   54 |
| ButtonConfigFast3               |   1678/   79 |  1068/   68 |
| Encoded4To2ButtonConfig         |   2160/   82 |  1550/   71 |
| Encoded8To3ButtonConfig         |   2428/  139 |  1818/  128 |
| EncodedButtonConfig             |   2474/  162 |  1864/  151 |
| LadderButtonConfig              |   2476/  175 |  1866/  164 |
+--------------------------------------------------------------+

```

## SparkFun Pro Micro

* 16 MHz ATmega32U4
* Arduino IDE 1.8.13
* SparkFun AVR Boards 1.1.13

```
+--------------------------------------------------------------+
| functionality                   |  flash/  ram |       delta |
|---------------------------------+--------------+-------------|
| Baseline                        |   3558/  151 |     0/    0 |
|---------------------------------+--------------+-------------|
| ButtonConfig                    |   4950/  191 |  1392/   40 |
| ButtonConfigFast1               |   4548/  191 |   990/   40 |
| ButtonConfigFast2               |   4512/  205 |   954/   54 |
| ButtonConfigFast3               |   4560/  219 |  1002/   68 |
| Encoded4To2ButtonConfig         |   5166/  222 |  1608/   71 |
| Encoded8To3ButtonConfig         |   5434/  279 |  1876/  128 |
| EncodedButtonConfig             |   5494/  300 |  1936/  149 |
| LadderButtonConfig              |   5544/  315 |  1986/  164 |
+--------------------------------------------------------------+

```

## SAMD21 M0 Mini

* 48 MHz ARM Cortex-M0+
* Arduino IDE 1.8.13
* SparkFun SAMD Core 1.8.1

```
+--------------------------------------------------------------+
| functionality                   |  flash/  ram |       delta |
|---------------------------------+--------------+-------------|
| Baseline                        |  10072/    0 |     0/    0 |
|---------------------------------+--------------+-------------|
| ButtonConfig                    |  11344/    0 |  1272/    0 |
| ButtonConfigFast1               |     -1/   -1 |    -1/   -1 |
| ButtonConfigFast2               |     -1/   -1 |    -1/   -1 |
| ButtonConfigFast3               |     -1/   -1 |    -1/   -1 |
| Encoded4To2ButtonConfig         |  11600/    0 |  1528/    0 |
| Encoded8To3ButtonConfig         |  11744/    0 |  1672/    0 |
| EncodedButtonConfig             |  11824/    0 |  1752/    0 |
| LadderButtonConfig              |  12080/    0 |  2008/    0 |
+--------------------------------------------------------------+

```

## ESP8266

* NodeMCU 1.0 clone, 80MHz ESP8266
* Arduino IDE 1.8.13
* ESP8266 Boards 2.7.4

```
+--------------------------------------------------------------+
| functionality                   |  flash/  ram |       delta |
|---------------------------------+--------------+-------------|
| Baseline                        | 256924/26800 |     0/    0 |
|---------------------------------+--------------+-------------|
| ButtonConfig                    | 258424/26840 |  1500/   40 |
| ButtonConfigFast1               |     -1/   -1 |    -1/   -1 |
| ButtonConfigFast2               |     -1/   -1 |    -1/   -1 |
| ButtonConfigFast3               |     -1/   -1 |    -1/   -1 |
| Encoded4To2ButtonConfig         | 258732/26916 |  1808/  116 |
| Encoded8To3ButtonConfig         | 258876/26980 |  1952/  180 |
| EncodedButtonConfig             | 259004/27020 |  2080/  220 |
| LadderButtonConfig              | 259048/27024 |  2124/  224 |
+--------------------------------------------------------------+

```

## STM32

* STM32 "Blue Pill", STM32F103C8, 72 MHz ARM Cortex-M3
* Arduino IDE 1.8.13
* STM32duino 1.9.0

```
+--------------------------------------------------------------+
| functionality                   |  flash/  ram |       delta |
|---------------------------------+--------------+-------------|
| Baseline                        |  19144/ 3788 |     0/    0 |
|---------------------------------+--------------+-------------|
| ButtonConfig                    |  21860/ 3848 |  2716/   60 |
| ButtonConfigFast1               |     -1/   -1 |    -1/   -1 |
| ButtonConfigFast2               |     -1/   -1 |    -1/   -1 |
| ButtonConfigFast3               |     -1/   -1 |    -1/   -1 |
| Encoded4To2ButtonConfig         |  22024/ 3884 |  2880/   96 |
| Encoded8To3ButtonConfig         |  22164/ 3948 |  3020/  160 |
| EncodedButtonConfig             |  22228/ 3956 |  3084/  168 |
| LadderButtonConfig              |  24948/ 3968 |  5804/  180 |
+--------------------------------------------------------------+

```

## ESP32

* ESP32-01 Dev Board, 240 MHz Tensilica LX6
* Arduino IDE 1.8.13
* ESP32 Boards 1.0.6

```
+--------------------------------------------------------------+
| functionality                   |  flash/  ram |       delta |
|---------------------------------+--------------+-------------|
| Baseline                        | 197914/13092 |     0/    0 |
|---------------------------------+--------------+-------------|
| ButtonConfig                    | 200822/13344 |  2908/  252 |
| ButtonConfigFast1               |     -1/   -1 |    -1/   -1 |
| ButtonConfigFast2               |     -1/   -1 |    -1/   -1 |
| ButtonConfigFast3               |     -1/   -1 |    -1/   -1 |
| Encoded4To2ButtonConfig         | 201042/13368 |  3128/  276 |
| Encoded8To3ButtonConfig         | 201170/13432 |  3256/  340 |
| EncodedButtonConfig             | 201274/13440 |  3360/  348 |
| LadderButtonConfig              | 216766/13472 | 18852/  380 |
+--------------------------------------------------------------+

```

## Teensy 3.2

* 96 MHz ARM Cortex-M4
* Arduino IDE 1.8.13
* Teensyduino 1.53
* Compiler options: "Faster"

```
+--------------------------------------------------------------+
| functionality                   |  flash/  ram |       delta |
|---------------------------------+--------------+-------------|
| Baseline                        |   7632/ 3048 |     0/    0 |
|---------------------------------+--------------+-------------|
| ButtonConfig                    |  12624/ 4200 |  4992/ 1152 |
| ButtonConfigFast1               |     -1/   -1 |    -1/   -1 |
| ButtonConfigFast2               |     -1/   -1 |    -1/   -1 |
| ButtonConfigFast3               |     -1/   -1 |    -1/   -1 |
| Encoded4To2ButtonConfig         |  12872/ 4260 |  5240/ 1212 |
| Encoded8To3ButtonConfig         |  13020/ 4324 |  5388/ 1276 |
| EncodedButtonConfig             |  13060/ 4332 |  5428/ 1284 |
| LadderButtonConfig              |  13708/ 4336 |  6076/ 1288 |
+--------------------------------------------------------------+

```

