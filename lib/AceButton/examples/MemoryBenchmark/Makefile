# See https://github.com/bxparks/EpoxyDuino for documentation about using
# EpoxyDuino to compile and run AUnit tests natively on Linux or MacOS.

APP_NAME := MemoryBenchmark
ARDUINO_LIBS := EpoxyMockDigitalWriteFast AceButton
MORE_CLEAN := more_clean
include ../../../EpoxyDuino/EpoxyDuino.mk

.PHONY: benchmarks

TARGETS := attiny.txt nano.txt micro.txt samd.txt stm32.txt esp8266.txt \
	esp32.txt teensy32.txt

README.md: $(TARGETS) generate_readme.py generate_table.awk
	./generate_readme.py > $@

benchmarks: $(TARGETS)

attiny.txt:
	./collect.sh attiny $@

nano.txt:
	./collect.sh nano $@

micro.txt:
	./collect.sh micro $@

samd.txt:
	./collect.sh samd $@

stm32.txt:
	./collect.sh stm32 $@

esp8266.txt:
	./collect.sh nodemcu $@

esp32.txt:
	./collect.sh esp32 $@

teensy32.txt:
	./collect.sh --ide teensy32 $@

more_clean:
	echo "Use 'make clean_benchmarks' to remove *.txt files"

clean_benchmarks:
	rm -f $(TARGETS)
