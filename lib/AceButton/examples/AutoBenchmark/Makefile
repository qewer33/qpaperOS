# See https://github.com/bxparks/EpoxyDuino for documentation about using
# EpoxyDuino to compile and run AUnit tests natively on Linux or MacOS.

APP_NAME := AutoBenchmark
ARDUINO_LIBS := EpoxyMockDigitalWriteFast AceCommon AceButton
MORE_CLEAN := more_clean
include ../../../EpoxyDuino/EpoxyDuino.mk

.PHONY: benchmarks

AUNITER_DIR := ../../../AUniter/tools

TARGETS := nano.txt micro.txt samd.txt stm32.txt esp8266.txt esp32.txt \
teensy32.txt

README.md: generate_readme.py generate_table.awk $(TARGETS)
	./generate_readme.py > $@

benchmarks: $(TARGETS)

# The USB/ACM ports can change dynamically. Make sure that the microcontroller
# is on the correct port before using these Make targets.
nano.txt:
	$(AUNITER_DIR)/auniter.sh upmon -o $@ --eof END nano:USB0

micro.txt:
	$(AUNITER_DIR)/auniter.sh upmon -o $@ --eof END micro:ACM0

samd.txt:
	$(AUNITER_DIR)/auniter.sh upmon -o $@ --eof END samd:ACM0

stm32.txt:
	$(AUNITER_DIR)/auniter.sh upmon -o $@ --eof END stm32:ACM0

esp8266.txt:
	$(AUNITER_DIR)/auniter.sh upmon -o $@ --eof END nodemcu:USB0

esp32.txt:
	$(AUNITER_DIR)/auniter.sh upmon -o $@ --eof END esp32:USB0

# Teensy requires manual capture of teensy32.txt. This Makefile rule is an
# aspirational hope for consistency.
teensy32.txt:
	$(AUNITER_DIR)/auniter.sh upmon -o $@ --eof END teensy32:ACM0

more_clean:
	echo "Use 'make clean_benchmarks' to remove *.txt files"

clean_benchmarks:
	rm -f $(TARGETS)
