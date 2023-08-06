/*
 * A program that prints out the time (min/avg/max) taken by the
 * AceButton::check() method. Prints the timing stats (min/avg/max in micros)
 * and the sample size in the following format:
 *
 * @verbatim
 * SIZEOF
 * sizeof(AceButton): 24
 * sizeof(ButtonConfig): 32
 * sizeof(Encoded4To2ButtonConfig): 40
 * sizeof(Encoded8To3ButtonConfig): 40
 * sizeof(EncodedButtonConfig): 56
 * sizeof(LadderButtonConfig): 56
 * BENCHMARKS
 * idle 0 1 12 924
 * press/release 0 1 15 922
 * click 0 1 12 923
 * double click 0 1 27 923
 * long press/repeat press 0 1 16 921
 * Encoded4To2ButtonConfig 0 2 36 921
 * Encoded8To3ButtonConfig 0 3 16 920
 * EncodedButtonConfig 0 3 16 921
 * LadderButtonConfig 0 3 426 921
 * END
 * @endverbatim
 *
 * Depends on:
 *
 *  * https://github.com/bxparks/AceCommon
 *  * https://github.com/bxparks/AceButton
 */

#include <AceCommon.h>
#include <AceButton.h>
#include "ProfilingButtonConfig.h"

#if defined(EPOXY_DUINO) || defined(ARDUINO_ARCH_AVR)
  #include <digitalWriteFast.h>
  #include <ace_button/fast/ButtonConfigFast1.h>
  #include <ace_button/fast/ButtonConfigFast2.h>
  #include <ace_button/fast/ButtonConfigFast3.h>
#endif
using ace_common::TimingStats;
using namespace ace_button;

// Set this to 1 to run the benchmarks using the IEventHandler object instead of
// EventHandler function. The result is that AceButton::check() seems slightly
// slower (maybe a few microseconds). Probably not enough to worry about.
#define USE_EVENT_HANDLER_CLASS 1

#if !defined(SERIAL_PORT_MONITOR)
#define SERIAL_PORT_MONITOR Serial
#endif

//----------------------------------------------------------------------------
// The pin number attached to the button.
const int BUTTON_PIN = 2;

// Create one button wired using the ProfilingButtonConfig, which is the same as
// ButtonConfig except readButton() is overridden for profiling purposes.
ProfilingButtonConfig buttonConfig;
AceButton simpleButton(&buttonConfig);

//----------------------------------------------------------------------------

#if defined(EPOXY_DUINO) || defined(ARDUINO_ARCH_AVR)
  // Create buttons for ButtonConfigFast{1,2,3}
  // Physical pin numbers attached to the buttons.
  const uint8_t BUTTON0_PHYSICAL_PIN = 4;
  const uint8_t BUTTON1_PHYSICAL_PIN = 5;
  const uint8_t BUTTON2_PHYSICAL_PIN = 6;

  // Virtual pin numbers attached to the buttons.
  const uint8_t BUTTON0_PIN = 0;
  const uint8_t BUTTON1_PIN = 1;
  const uint8_t BUTTON2_PIN = 2;

  ButtonConfigFast1<BUTTON0_PHYSICAL_PIN> buttonConfigFast1;
  AceButton buttonF1B0(&buttonConfigFast1, BUTTON0_PIN);

  ButtonConfigFast2<
      BUTTON0_PHYSICAL_PIN,
      BUTTON1_PHYSICAL_PIN
  > buttonConfigFast2;
  AceButton buttonF2B0(&buttonConfigFast2, BUTTON0_PIN);
  AceButton buttonF2B1(&buttonConfigFast2, BUTTON1_PIN);

  ButtonConfigFast3<
      BUTTON0_PHYSICAL_PIN,
      BUTTON1_PHYSICAL_PIN,
      BUTTON2_PHYSICAL_PIN
  > buttonConfigFast3;
  AceButton buttonF3B0(&buttonConfigFast3, BUTTON0_PIN);
  AceButton buttonF3B1(&buttonConfigFast3, BUTTON1_PIN);
  AceButton buttonF3B2(&buttonConfigFast3, BUTTON2_PIN);

#endif

//----------------------------------------------------------------------------

// Create 3 buttons for Encoded4To2ButtonConfig
static const uint8_t BUTTON_PIN0 = 2;
static const uint8_t BUTTON_PIN1 = 3;
static const uint8_t BUTTON_PIN2 = 4;
Encoded4To2ButtonConfig encoded4To2ButtonConfig(BUTTON_PIN0, BUTTON_PIN1);
AceButton four1(&encoded4To2ButtonConfig, 1);
AceButton four2(&encoded4To2ButtonConfig, 2);
AceButton four3(&encoded4To2ButtonConfig, 3);

// Create 7 buttons for Encoded8To3ButtonConfig
Encoded8To3ButtonConfig encoded8To3ButtonConfig(
    BUTTON_PIN0, BUTTON_PIN1, BUTTON_PIN2);
AceButton eight1(&encoded8To3ButtonConfig, 1);
AceButton eight2(&encoded8To3ButtonConfig, 2);
AceButton eight3(&encoded8To3ButtonConfig, 3);
AceButton eight4(&encoded8To3ButtonConfig, 4);
AceButton eight5(&encoded8To3ButtonConfig, 5);
AceButton eight6(&encoded8To3ButtonConfig, 6);
AceButton eight7(&encoded8To3ButtonConfig, 7);

// Create an array of 7 buttons wired to use the EncodedButtonConfig using
// 4 digital pins.
static const uint8_t NUM_PINS = 3;
static const uint8_t PINS[] = {2, 3, 4};
static const uint8_t NUM_BUTTONS = 7;
// e00 represents "no button pressed"
static AceButton e01(nullptr, 1);
static AceButton e02(nullptr, 2);
static AceButton e03(nullptr, 3);
static AceButton e04(nullptr, 4);
static AceButton e05(nullptr, 5);
static AceButton e06(nullptr, 6);
static AceButton e07(nullptr, 7);
static AceButton* const ENCODED_BUTTONS[NUM_BUTTONS] = {
    &e01, &e02, &e03, &e04, &e05, &e06, &e07,
};
static EncodedButtonConfig encodedButtonConfig(
  NUM_PINS, PINS, NUM_BUTTONS, ENCODED_BUTTONS
);

// Create a LadderButtonConfig with 8 levels for 7 buttons.
static const int ANALOG_BUTTON_PIN = A0;
static const uint8_t NUM_LEVELS = NUM_BUTTONS + 1;
static const uint16_t LEVELS[NUM_LEVELS] = {
  0   /* 0%, short to ground */,
  133 /* 13%, 1.5 kohm */,
  327 /* 32%, 4.7 kohm */,
  512 /* 50%, 10 kohm */,
  614 /* 60%, 15 kohm */,
  788 /* 77%, 33 kohm */,
  930 /* 91%, 100 kohm */,
  1023 /* 100%, open circuit */,
};
static AceButton r00(nullptr, 0);
static AceButton r01(nullptr, 1);
static AceButton r02(nullptr, 2);
static AceButton r03(nullptr, 3);
static AceButton r04(nullptr, 4);
static AceButton r05(nullptr, 5);
static AceButton r06(nullptr, 6);
// r07 represents "no button pressed".
static AceButton* const LADDER_BUTTONS[NUM_BUTTONS] = {
    &r00, &r01, &r02, &r03, &r04, &r05, &r06,
};
static LadderButtonConfig ladderButtonConfig(
  ANALOG_BUTTON_PIN, NUM_LEVELS, LEVELS, NUM_BUTTONS, LADDER_BUTTONS
);

const unsigned long STATS_PRINT_INTERVAL = 2000;
TimingStats stats;

const uint8_t LOOP_MODE_START = 0;
const uint8_t LOOP_MODE_IDLE = 1;
const uint8_t LOOP_MODE_PRESS_RELEASE = 2;
const uint8_t LOOP_MODE_CLICK = 3;
const uint8_t LOOP_MODE_DOUBLE_CLICK = 4;
const uint8_t LOOP_MODE_LONG_PRESS = 5;
const uint8_t LOOP_MODE_BUTTON_CONFIG_FAST1 = 6;
const uint8_t LOOP_MODE_BUTTON_CONFIG_FAST2 = 7;
const uint8_t LOOP_MODE_BUTTON_CONFIG_FAST3 = 8;
const uint8_t LOOP_MODE_ENCODED_4TO2_BUTTON_CONFIG = 9;
const uint8_t LOOP_MODE_ENCODED_8TO3_BUTTON_CONFIG = 10;
const uint8_t LOOP_MODE_ENCODED_BUTTON_CONFIG = 11;
const uint8_t LOOP_MODE_LADDER_BUTTON_CONFIG = 12;
const uint8_t LOOP_MODE_END = 13;
uint8_t loopMode;
uint8_t loopEventType;

//-----------------------------------------------------------------------------

void printStats(const __FlashStringHelper* label) {
  SERIAL_PORT_MONITOR.print(label);
  SERIAL_PORT_MONITOR.print(' ');
  SERIAL_PORT_MONITOR.print(stats.getMin());
  SERIAL_PORT_MONITOR.print(' ');
  SERIAL_PORT_MONITOR.print(stats.getAvg());
  SERIAL_PORT_MONITOR.print(' ');
  SERIAL_PORT_MONITOR.print(stats.getMax());
  SERIAL_PORT_MONITOR.print(' ');
  SERIAL_PORT_MONITOR.print(stats.getCount());
  SERIAL_PORT_MONITOR.println();
}

void nextMode() {
  stats.reset();
  buttonConfig.setButtonState(HIGH);
  loopMode++;
}

#if USE_EVENT_HANDLER_CLASS
  class ButtonHandler: public IEventHandler {
    public:
      void handleEvent(AceButton* /* button */, uint8_t eventType,
          uint8_t /* buttonState */) override {
        loopEventType = eventType;
      }
  };

  ButtonHandler handleEvent;
#else
  // An empty event handler.
  void handleEvent(AceButton* /* button */, uint8_t eventType,
      uint8_t /* buttonState */) {
    loopEventType = eventType;
  }
#endif

//-----------------------------------------------------------------------------

// Measure how long the AceButton.check() takes
void checkSimpleButton() {
  uint16_t startMicros = micros();
  simpleButton.check();
  uint16_t elapsedMicros = micros() - startMicros;
  stats.update(elapsedMicros);
}

#if defined(EPOXY_DUINO) || defined(ARDUINO_ARCH_AVR)
  void checkButtonConfigFast1() {
    uint16_t startMicros = micros();
    buttonF1B0.check();
    uint16_t elapsedMicros = micros() - startMicros;
    stats.update(elapsedMicros);
  }

  void checkButtonConfigFast2() {
    uint16_t startMicros = micros();
    buttonF2B0.check();
    buttonF2B1.check();
    uint16_t elapsedMicros = micros() - startMicros;
    stats.update(elapsedMicros);
  }

  void checkButtonConfigFast3() {
    uint16_t startMicros = micros();
    buttonF3B0.check();
    buttonF3B1.check();
    buttonF3B2.check();
    uint16_t elapsedMicros = micros() - startMicros;
    stats.update(elapsedMicros);
  }
#endif

void checkEncoded4To2Buttons() {
  uint16_t startMicros = micros();
  four1.check();
  four2.check();
  four3.check();
  uint16_t elapsedMicros = micros() - startMicros;
  stats.update(elapsedMicros);
}

void checkEncoded8To3Buttons() {
  uint16_t startMicros = micros();
  eight1.check();
  eight2.check();
  eight3.check();
  eight4.check();
  eight5.check();
  eight6.check();
  eight7.check();
  uint16_t elapsedMicros = micros() - startMicros;
  stats.update(elapsedMicros);
}

void checkEncodedButtons() {
  uint16_t startMicros = micros();
  encodedButtonConfig.checkButtons();
  uint16_t elapsedMicros = micros() - startMicros;
  stats.update(elapsedMicros);
}

void checkLadderButtons() {
  uint16_t startMicros = micros();
  ladderButtonConfig.checkButtons();
  uint16_t elapsedMicros = micros() - startMicros;
  stats.update(elapsedMicros);
}

//-----------------------------------------------------------------------------

void loopStartBenchmarks() {
  SERIAL_PORT_MONITOR.println(F("BENCHMARKS"));
  nextMode();
}

void loopIdle() {
  static unsigned long start = millis();

  checkSimpleButton();

  if (millis() - start > STATS_PRINT_INTERVAL) {
    printStats(F("idle"));
    nextMode();
  }
}

void loopPressRelease() {
  static unsigned long start = millis();

  checkSimpleButton();

  unsigned long now = millis();
  unsigned long elapsed = now - start;
  if (100 <= elapsed && elapsed < 1000) buttonConfig.setButtonState(LOW);
  if (1000 <= elapsed) buttonConfig.setButtonState(HIGH);

  if (millis() - start > STATS_PRINT_INTERVAL) {
    if (loopEventType != AceButton::kEventReleased) {
      SERIAL_PORT_MONITOR.print(F("ERROR "));
    }
    printStats(F("press/release"));
    nextMode();
  }
}

void loopClick() {
  static unsigned long start = millis();

  checkSimpleButton();

  unsigned long now = millis();
  unsigned long elapsed = now - start;
  if (100 <= elapsed && elapsed < 200) buttonConfig.setButtonState(LOW);
  if (200 <= elapsed) buttonConfig.setButtonState(HIGH);

  if (millis() - start > STATS_PRINT_INTERVAL) {
    if (loopEventType != AceButton::kEventClicked) {
      SERIAL_PORT_MONITOR.print(F("ERROR "));
    }
    printStats(F("click"));
    nextMode();
  }
}

void loopDoubleClick() {
  static unsigned long start = millis();

  checkSimpleButton();

  unsigned long now = millis();
  unsigned long elapsed = now - start;
  if (100 <= elapsed && elapsed < 200) buttonConfig.setButtonState(LOW);
  if (200 <= elapsed && elapsed < 300) buttonConfig.setButtonState(HIGH);
  if (300 <= elapsed && elapsed < 400) buttonConfig.setButtonState(LOW);
  if (400 <= elapsed) buttonConfig.setButtonState(HIGH);

  if (millis() - start > STATS_PRINT_INTERVAL) {
    if (loopEventType != AceButton::kEventDoubleClicked) {
      SERIAL_PORT_MONITOR.print(F("ERROR "));
    }
    printStats(F("double_click"));
    nextMode();
  }
}

void loopLongPress() {
  static unsigned long start = millis();

  checkSimpleButton();

  unsigned long now = millis();
  unsigned long elapsed = now - start;
  if (100 <= elapsed) buttonConfig.setButtonState(LOW);

  if (millis() - start > STATS_PRINT_INTERVAL) {
    if (loopEventType != AceButton::kEventRepeatPressed) {
      SERIAL_PORT_MONITOR.print(F("ERROR "));
    }
    printStats(F("long_press/repeat_press"));
    nextMode();
  }
}

#if defined(EPOXY_DUINO) || defined(ARDUINO_ARCH_AVR)
void loopCheckButtonConfigFast1() {
  static unsigned long start = millis();

  checkButtonConfigFast1();

  if (millis() - start > STATS_PRINT_INTERVAL) {
    printStats(F("ButtonConfigFast1"));
    nextMode();
  }
}

void loopCheckButtonConfigFast2() {
  static unsigned long start = millis();

  checkButtonConfigFast2();

  if (millis() - start > STATS_PRINT_INTERVAL) {
    printStats(F("ButtonConfigFast2"));
    nextMode();
  }
}

void loopCheckButtonConfigFast3() {
  static unsigned long start = millis();

  checkButtonConfigFast3();

  if (millis() - start > STATS_PRINT_INTERVAL) {
    printStats(F("ButtonConfigFast3"));
    nextMode();
  }
}

#endif

void loopEncoded4To2ButtonConfig() {
  static unsigned long start = millis();

  checkEncoded4To2Buttons();

  if (millis() - start > STATS_PRINT_INTERVAL) {
    printStats(F("Encoded4To2ButtonConfig"));
    nextMode();
  }
}

void loopEncoded8To3ButtonConfig() {
  static unsigned long start = millis();

  checkEncoded8To3Buttons();

  if (millis() - start > STATS_PRINT_INTERVAL) {
    printStats(F("Encoded8To3ButtonConfig"));
    nextMode();
  }
}

void loopEncodedButtonConfig() {
  static unsigned long start = millis();

  checkEncodedButtons();

  if (millis() - start > STATS_PRINT_INTERVAL) {
    printStats(F("EncodedButtonConfig"));
    nextMode();
  }
}

void loopLadderButtonConfig() {
  static unsigned long start = millis();

  checkLadderButtons();

  if (millis() - start > STATS_PRINT_INTERVAL) {
    printStats(F("LadderButtonConfig"));
    nextMode();
  }
}

void loopEndBenchmarks() {
  SERIAL_PORT_MONITOR.println(F("END"));
  nextMode();
}

//-----------------------------------------------------------------------------

void setup() {
#if ! defined(EPOXY_DUINO)
  delay(1000); // some microcontrollers reboot twice
#endif

  SERIAL_PORT_MONITOR.begin(115200);
  while (!SERIAL_PORT_MONITOR); // wait until ready - Leonardo/Micro

  SERIAL_PORT_MONITOR.println(F("SIZEOF"));

  // Print sizeof various classes
  SERIAL_PORT_MONITOR.print(F("sizeof(AceButton): "));
  SERIAL_PORT_MONITOR.println(sizeof(AceButton));

  SERIAL_PORT_MONITOR.print(F("sizeof(ButtonConfig): "));
  SERIAL_PORT_MONITOR.println(sizeof(ButtonConfig));

#if defined(EPOXY_DUINO) || defined(ARDUINO_ARCH_AVR)
  SERIAL_PORT_MONITOR.print(F("sizeof(ButtonConfigFast1<>): "));
  SERIAL_PORT_MONITOR.println(sizeof(ButtonConfigFast1<1>));

  SERIAL_PORT_MONITOR.print(F("sizeof(ButtonConfigFast2<>): "));
  SERIAL_PORT_MONITOR.println(sizeof(ButtonConfigFast2<1, 2>));

  SERIAL_PORT_MONITOR.print(F("sizeof(ButtonConfigFast3<>): "));
  SERIAL_PORT_MONITOR.println(sizeof(ButtonConfigFast3<1, 2, 3>));
#endif

  SERIAL_PORT_MONITOR.print(F("sizeof(Encoded4To2ButtonConfig): "));
  SERIAL_PORT_MONITOR.println(sizeof(Encoded4To2ButtonConfig));

  SERIAL_PORT_MONITOR.print(F("sizeof(Encoded8To3ButtonConfig): "));
  SERIAL_PORT_MONITOR.println(sizeof(Encoded8To3ButtonConfig));

  SERIAL_PORT_MONITOR.print(F("sizeof(EncodedButtonConfig): "));
  SERIAL_PORT_MONITOR.println(sizeof(EncodedButtonConfig));

  SERIAL_PORT_MONITOR.print(F("sizeof(LadderButtonConfig): "));
  SERIAL_PORT_MONITOR.println(sizeof(LadderButtonConfig));

  // Button uses the built-in pull up register.
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  simpleButton.init(BUTTON_PIN);

  // Configure the ButtonConfig with the event handler, and enable all higher
  // level events.
#if USE_EVENT_HANDLER_CLASS
  buttonConfig.setIEventHandler(&handleEvent);
#else
  buttonConfig.setEventHandler(handleEvent);
#endif
  buttonConfig.setFeature(ButtonConfig::kFeatureClick);
  buttonConfig.setFeature(ButtonConfig::kFeatureDoubleClick);
  buttonConfig.setFeature(ButtonConfig::kFeatureLongPress);
  buttonConfig.setFeature(ButtonConfig::kFeatureRepeatPress);
  buttonConfig.setFeature(ButtonConfig::kFeatureSuppressAll);

#if defined(EPOXY_DUINO) || defined(ARDUINO_ARCH_AVR)
    // Configure ButtonConfigFast1
  #if USE_EVENT_HANDLER_CLASS
    buttonConfigFast1.setIEventHandler(&handleEvent);
  #else
    buttonConfigFast1.setEventHandler(handleEvent);
  #endif
    buttonConfigFast1.setFeature(ButtonConfig::kFeatureClick);
    buttonConfigFast1.setFeature(ButtonConfig::kFeatureDoubleClick);
    buttonConfigFast1.setFeature(ButtonConfig::kFeatureLongPress);
    buttonConfigFast1.setFeature(ButtonConfig::kFeatureRepeatPress);
    buttonConfigFast1.setFeature(ButtonConfig::kFeatureSuppressAll);

    // Configure ButtonConfigFast2
  #if USE_EVENT_HANDLER_CLASS
    buttonConfigFast2.setIEventHandler(&handleEvent);
  #else
    buttonConfigFast2.setEventHandler(handleEvent);
  #endif
    buttonConfigFast2.setFeature(ButtonConfig::kFeatureClick);
    buttonConfigFast2.setFeature(ButtonConfig::kFeatureDoubleClick);
    buttonConfigFast2.setFeature(ButtonConfig::kFeatureLongPress);
    buttonConfigFast2.setFeature(ButtonConfig::kFeatureRepeatPress);
    buttonConfigFast2.setFeature(ButtonConfig::kFeatureSuppressAll);

    // Configure ButtonConfigFast1
  #if USE_EVENT_HANDLER_CLASS
    buttonConfigFast3.setIEventHandler(&handleEvent);
  #else
    buttonConfigFast3.setEventHandler(handleEvent);
  #endif
    buttonConfigFast3.setFeature(ButtonConfig::kFeatureClick);
    buttonConfigFast3.setFeature(ButtonConfig::kFeatureDoubleClick);
    buttonConfigFast3.setFeature(ButtonConfig::kFeatureLongPress);
    buttonConfigFast3.setFeature(ButtonConfig::kFeatureRepeatPress);
    buttonConfigFast3.setFeature(ButtonConfig::kFeatureSuppressAll);
#endif

#if USE_EVENT_HANDLER_CLASS
  encoded4To2ButtonConfig.setIEventHandler(&handleEvent);
#else
  encoded4To2ButtonConfig.setEventHandler(handleEvent);
#endif
  encoded4To2ButtonConfig.setFeature(ButtonConfig::kFeatureClick);
  encoded4To2ButtonConfig.setFeature(ButtonConfig::kFeatureDoubleClick);
  encoded4To2ButtonConfig.setFeature(ButtonConfig::kFeatureLongPress);
  encoded4To2ButtonConfig.setFeature(ButtonConfig::kFeatureRepeatPress);
  encoded4To2ButtonConfig.setFeature(ButtonConfig::kFeatureSuppressAll);

#if USE_EVENT_HANDLER_CLASS
  encoded8To3ButtonConfig.setIEventHandler(&handleEvent);
#else
  encoded8To3ButtonConfig.setEventHandler(handleEvent);
#endif
  encoded8To3ButtonConfig.setFeature(ButtonConfig::kFeatureClick);
  encoded8To3ButtonConfig.setFeature(ButtonConfig::kFeatureDoubleClick);
  encoded8To3ButtonConfig.setFeature(ButtonConfig::kFeatureLongPress);
  encoded8To3ButtonConfig.setFeature(ButtonConfig::kFeatureRepeatPress);
  encoded8To3ButtonConfig.setFeature(ButtonConfig::kFeatureSuppressAll);

#if USE_EVENT_HANDLER_CLASS
  encodedButtonConfig.setIEventHandler(&handleEvent);
#else
  encodedButtonConfig.setEventHandler(handleEvent);
#endif
  encodedButtonConfig.setFeature(ButtonConfig::kFeatureClick);
  encodedButtonConfig.setFeature(ButtonConfig::kFeatureDoubleClick);
  encodedButtonConfig.setFeature(ButtonConfig::kFeatureLongPress);
  encodedButtonConfig.setFeature(ButtonConfig::kFeatureRepeatPress);
  encodedButtonConfig.setFeature(ButtonConfig::kFeatureSuppressAll);

#if USE_EVENT_HANDLER_CLASS
  ladderButtonConfig.setIEventHandler(&handleEvent);
#else
  ladderButtonConfig.setEventHandler(handleEvent);
#endif
  ladderButtonConfig.setFeature(ButtonConfig::kFeatureClick);
  ladderButtonConfig.setFeature(ButtonConfig::kFeatureDoubleClick);
  ladderButtonConfig.setFeature(ButtonConfig::kFeatureLongPress);
  ladderButtonConfig.setFeature(ButtonConfig::kFeatureRepeatPress);
  ladderButtonConfig.setFeature(ButtonConfig::kFeatureSuppressAll);

  loopMode = LOOP_MODE_START;
  loopEventType = AceButton::kEventPressed;
}

void loop() {
  delay(1); // Decrease sampling frequency to about 1000 Hz

  switch (loopMode) {
    case LOOP_MODE_START:
      loopStartBenchmarks();
      break;
    case LOOP_MODE_IDLE:
      loopIdle();
      break;
    case LOOP_MODE_PRESS_RELEASE:
      loopPressRelease();
      break;
    case LOOP_MODE_CLICK:
      loopClick();
      break;
    case LOOP_MODE_DOUBLE_CLICK:
      loopDoubleClick();
      break;
    case LOOP_MODE_LONG_PRESS:
      loopLongPress();
      break;

    case LOOP_MODE_BUTTON_CONFIG_FAST1:
      #if defined(EPOXY_DUINO) || defined(ARDUINO_ARCH_AVR)
        loopCheckButtonConfigFast1();
      #else
        nextMode();
      #endif
      break;
    case LOOP_MODE_BUTTON_CONFIG_FAST2:
      #if defined(EPOXY_DUINO) || defined(ARDUINO_ARCH_AVR)
        loopCheckButtonConfigFast2();
      #else
        nextMode();
      #endif
      break;
    case LOOP_MODE_BUTTON_CONFIG_FAST3:
      #if defined(EPOXY_DUINO) || defined(ARDUINO_ARCH_AVR)
        loopCheckButtonConfigFast3();
      #else
        nextMode();
      #endif
      break;

    case LOOP_MODE_ENCODED_4TO2_BUTTON_CONFIG:
      loopEncoded4To2ButtonConfig();
      break;
    case LOOP_MODE_ENCODED_8TO3_BUTTON_CONFIG:
      loopEncoded8To3ButtonConfig();
      break;
    case LOOP_MODE_ENCODED_BUTTON_CONFIG:
      loopEncodedButtonConfig();
      break;
    case LOOP_MODE_LADDER_BUTTON_CONFIG:
      loopLadderButtonConfig();
      break;
    case LOOP_MODE_END:
      loopEndBenchmarks();
      break;
    default:
      #ifdef EPOXY_DUINO
        exit(1);
      #endif
      break;
  }
}
