/*
 * A program which compiles in different ButtonConfig instances so that the
 * flash and static memory sizes can be extract from the output of the compiler.
 * Set the FEATURE macro to various integers to compile different parts of the
 * AceButton library.
 */

#include <Arduino.h>

// List of features of the AceButton library that we want to examine.
#define FEATURE_BASELINE 0
#define FEATURE_BUTTON_CONFIG 1
#define FEATURE_BUTTON_CONFIG_FAST1 2
#define FEATURE_BUTTON_CONFIG_FAST2 3
#define FEATURE_BUTTON_CONFIG_FAST3 4
#define FEATURE_ENCODED_4TO2_BUTTON_CONFIG 5
#define FEATURE_ENCODED_8TO3_BUTTON_CONFIG 6
#define FEATURE_ENCODED_BUTTON_CONFIG 7
#define FEATURE_LADDER_BUTTON_CONFIG 8

// Select one of the FEATURE_* parameters and compile. Then look at the flash
// and RAM usage, compared to FEATURE_BASELINE usage to determine how much
// flash and RAM is consumed by the selected feature.
// NOTE: This line is modified by a 'sed' script in collect.sh. Be careful
// when modifying its format.
#define FEATURE 0

// A volatile integer to prevent the compiler from optimizing away the entire
// program.
volatile int disableCompilerOptimization = 0;

#if FEATURE != FEATURE_BASELINE
  #include <AceButton.h>
  using namespace ace_button;

void handleEvent(AceButton* /* button */, uint8_t /* eventType */,
    uint8_t /* buttonState */) {
  disableCompilerOptimization++;
}
#endif

#if FEATURE == FEATURE_BUTTON_CONFIG
  static const int BUTTON_PIN = 2;
  AceButton button(BUTTON_PIN);

#elif FEATURE == FEATURE_BUTTON_CONFIG_FAST1
  #if ! defined(ARDUINO_ARCH_AVR) && ! defined(EPOXY_DUINO)
    #error Unsupported FEATURE on this platform
  #endif

  #include <digitalWriteFast.h>
  #include <ace_button/fast/ButtonConfigFast1.h>

  static const int BUTTON1_PHYSICAL_PIN = 2;
  static const int BUTTON1_PIN = 0;
  ButtonConfigFast1<BUTTON1_PHYSICAL_PIN> buttonConfig;
  AceButton button1(&buttonConfig, BUTTON1_PIN);

#elif FEATURE == FEATURE_BUTTON_CONFIG_FAST2
  #if ! defined(ARDUINO_ARCH_AVR) && ! defined(EPOXY_DUINO)
    #error Unsupported FEATURE on this platform
  #endif

  #include <digitalWriteFast.h>
  #include <ace_button/fast/ButtonConfigFast2.h>

  static const int BUTTON1_PHYSICAL_PIN = 2;
  static const int BUTTON2_PHYSICAL_PIN = 3;
  static const int BUTTON1_PIN = 0;
  static const int BUTTON2_PIN = 1;
  ButtonConfigFast2<BUTTON1_PHYSICAL_PIN, BUTTON2_PHYSICAL_PIN> buttonConfig;
  AceButton button1(&buttonConfig, BUTTON1_PIN);
  AceButton button2(&buttonConfig, BUTTON2_PIN);

#elif FEATURE == FEATURE_BUTTON_CONFIG_FAST3
  #if ! defined(ARDUINO_ARCH_AVR) && ! defined(EPOXY_DUINO)
    #error Unsupported FEATURE on this platform
  #endif

  #include <digitalWriteFast.h>
  #include <ace_button/fast/ButtonConfigFast3.h>

  static const int BUTTON1_PHYSICAL_PIN = 2;
  static const int BUTTON2_PHYSICAL_PIN = 3;
  static const int BUTTON3_PHYSICAL_PIN = 4;
  static const int BUTTON1_PIN = 0;
  static const int BUTTON2_PIN = 1;
  static const int BUTTON3_PIN = 2;
  ButtonConfigFast3<
      BUTTON1_PHYSICAL_PIN,
      BUTTON2_PHYSICAL_PIN,
      BUTTON3_PHYSICAL_PIN
  > buttonConfig;
  AceButton button1(&buttonConfig, BUTTON1_PIN);
  AceButton button2(&buttonConfig, BUTTON2_PIN);
  AceButton button3(&buttonConfig, BUTTON3_PIN);

#elif FEATURE == FEATURE_ENCODED_4TO2_BUTTON_CONFIG
  static const uint8_t BUTTON_PIN0 = 2;
  static const uint8_t BUTTON_PIN1 = 3;
  Encoded4To2ButtonConfig buttonConfig(BUTTON_PIN0, BUTTON_PIN1);

  AceButton b1(&buttonConfig, 1);
  AceButton b2(&buttonConfig, 2);
  AceButton b3(&buttonConfig, 3);

#elif FEATURE == FEATURE_ENCODED_8TO3_BUTTON_CONFIG
  static const uint8_t BUTTON_PIN0 = 2;
  static const uint8_t BUTTON_PIN1 = 3;
  static const uint8_t BUTTON_PIN2 = 4;
  Encoded8To3ButtonConfig buttonConfig(BUTTON_PIN0, BUTTON_PIN1, BUTTON_PIN2);

  AceButton b1(&buttonConfig, 1);
  AceButton b2(&buttonConfig, 2);
  AceButton b3(&buttonConfig, 3);
  AceButton b4(&buttonConfig, 4);
  AceButton b5(&buttonConfig, 5);
  AceButton b6(&buttonConfig, 6);
  AceButton b7(&buttonConfig, 7);

#elif FEATURE == FEATURE_ENCODED_BUTTON_CONFIG
  static const uint8_t NUM_PINS = 3;
  static const uint8_t PINS[] = {2, 3, 4};
  static const uint8_t NUM_BUTTONS = 7;
  // button 0 cannot be used because it represents "no button pressed"
  static AceButton b01(nullptr, 1);
  static AceButton b02(nullptr, 2);
  static AceButton b03(nullptr, 3);
  static AceButton b04(nullptr, 4);
  static AceButton b05(nullptr, 5);
  static AceButton b06(nullptr, 6);
  static AceButton b07(nullptr, 7);
  static AceButton* const BUTTONS[NUM_BUTTONS] = {
      &b01, &b02, &b03, &b04, &b05, &b06, &b07,
  };
  static EncodedButtonConfig buttonConfig(
    NUM_PINS, PINS, NUM_BUTTONS, BUTTONS
  );

#elif FEATURE == FEATURE_LADDER_BUTTON_CONFIG
  static const uint8_t NUM_PINS = 3;
  static const uint8_t PINS[] = {2, 3, 4};
  static const uint8_t NUM_BUTTONS = 7;
  static AceButton b00(nullptr, 0);
  static AceButton b01(nullptr, 1);
  static AceButton b02(nullptr, 2);
  static AceButton b03(nullptr, 3);
  static AceButton b04(nullptr, 4);
  static AceButton b05(nullptr, 5);
  static AceButton b06(nullptr, 6);
  // button 7 cannot be used because it represents "no button pressed"
  static AceButton* const BUTTONS[NUM_BUTTONS] = {
      &b00, &b01, &b02, &b03, &b04, &b05, &b06,
  };

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
  static LadderButtonConfig buttonConfig(
    ANALOG_BUTTON_PIN, NUM_LEVELS, LEVELS, NUM_BUTTONS, BUTTONS
  );
#endif

void setup() {
  delay(2000);

#if FEATURE == FEATURE_BUTTON_CONFIG
  pinMode(BUTTON_PIN, INPUT_PULLUP);
#elif FEATURE == FEATURE_BUTTON_CONFIG_FAST1
  pinModeFast(BUTTON1_PIN, INPUT_PULLUP);
#elif FEATURE == FEATURE_BUTTON_CONFIG_FAST2
  pinModeFast(BUTTON1_PIN, INPUT_PULLUP);
  pinModeFast(BUTTON2_PIN, INPUT_PULLUP);
#elif FEATURE == FEATURE_BUTTON_CONFIG_FAST3
  pinModeFast(BUTTON1_PIN, INPUT_PULLUP);
  pinModeFast(BUTTON2_PIN, INPUT_PULLUP);
  pinModeFast(BUTTON3_PIN, INPUT_PULLUP);
#elif FEATURE == FEATURE_ENCODED_4TO2_BUTTON_CONFIG
  pinMode(BUTTON_PIN0, INPUT_PULLUP);
  pinMode(BUTTON_PIN1, INPUT_PULLUP);
#elif FEATURE == FEATURE_ENCODED_8TO3_BUTTON_CONFIG
  pinMode(BUTTON_PIN0, INPUT_PULLUP);
  pinMode(BUTTON_PIN1, INPUT_PULLUP);
  pinMode(BUTTON_PIN2, INPUT_PULLUP);
#elif FEATURE == FEATURE_ENCODED_BUTTON_CONFIG
  pinMode(PINS[0], INPUT_PULLUP);
  pinMode(PINS[1], INPUT_PULLUP);
  pinMode(PINS[2], INPUT_PULLUP);
#elif FEATURE == FEATURE_LADDER_BUTTON_CONFIG
  pinMode(ANALOG_BUTTON_PIN, INPUT);
#endif

  // Configure the ButtonConfig with the event handler, and enable all higher
  // level events.
#if FEATURE == FEATURE_BUTTON_CONFIG
  ButtonConfig* config = button.getButtonConfig();
#elif FEATURE > FEATURE_BUTTON_CONFIG
  ButtonConfig* config = &buttonConfig;
#endif

#if FEATURE != FEATURE_BASELINE
  config->setEventHandler(handleEvent);
  config->setFeature(ButtonConfig::kFeatureClick);
  config->setFeature(ButtonConfig::kFeatureDoubleClick);
  config->setFeature(ButtonConfig::kFeatureLongPress);
  config->setFeature(ButtonConfig::kFeatureRepeatPress);
  config->setFeature(ButtonConfig::kFeatureSuppressAll);
#endif
}

void loop() {
#if FEATURE == FEATURE_BASELINE
  disableCompilerOptimization++;
#elif FEATURE == FEATURE_BUTTON_CONFIG
  button.check();
#elif FEATURE == FEATURE_BUTTON_CONFIG_FAST1
  button1.check();
#elif FEATURE == FEATURE_BUTTON_CONFIG_FAST2
  button1.check();
  button2.check();
#elif FEATURE == FEATURE_BUTTON_CONFIG_FAST3
  button1.check();
  button2.check();
  button3.check();
#elif FEATURE == FEATURE_ENCODED_4TO2_BUTTON_CONFIG
  b1.check();
  b2.check();
  b3.check();
#elif FEATURE == FEATURE_ENCODED_8TO3_BUTTON_CONFIG
  b1.check();
  b2.check();
  b3.check();
  b4.check();
  b5.check();
  b6.check();
  b7.check();
#elif FEATURE == FEATURE_ENCODED_BUTTON_CONFIG
  buttonConfig.checkButtons();
#elif FEATURE == FEATURE_LADDER_BUTTON_CONFIG
  buttonConfig.checkButtons();
#endif
}
