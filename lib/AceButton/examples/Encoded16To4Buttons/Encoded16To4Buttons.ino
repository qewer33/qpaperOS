/*
 * A demo of EncodedButtonConfig which can support (almost) arbitrary number of
 * buttons. This particular example shows how to handle 15 buttons using 4 GPIO
 * pins.
 */

#include <AceButton.h>
using namespace ace_button;

#ifdef ESP32
  // Different ESP32 boards use different pins
  static const int LED_PIN = 2;
#else
  static const int LED_PIN = LED_BUILTIN;
#endif

// LED states. Some microcontrollers wire their built-in LED the reverse.
static const int LED_ON = HIGH;
static const int LED_OFF = LOW;

// Create an array of actual pins used.
static const uint8_t NUM_PINS = 4;
static const uint8_t PINS[] = {2, 3, 4, 5};

// Create 15 AceButton objects manually with virtual pin numbers 1 to 15.
// Virtual pin number 0 cannot be used because it represents "no button
// pressed". We could use an array of AceButton BUTTONS[15], and use a loop to
// initialize these arrays, but this is more explicit and easier to understand
// as an example code.
//
// We use the 4-parameter AceButton() constructor with the `buttonConfig`
// parameter set to `nullptr` to prevent the creation of the default
// SystemButtonConfig which will never be used. This saves about 30 bytes of
// flash and 26 bytes of static RAM on an AVR processor.
static const uint8_t NUM_BUTTONS = 15;
static AceButton b01(nullptr, 1);
static AceButton b02(nullptr, 2);
static AceButton b03(nullptr, 3);
static AceButton b04(nullptr, 4);
static AceButton b05(nullptr, 5);
static AceButton b06(nullptr, 6);
static AceButton b07(nullptr, 7);
static AceButton b08(nullptr, 8);
static AceButton b09(nullptr, 9);
static AceButton b10(nullptr, 10);
static AceButton b11(nullptr, 11);
static AceButton b12(nullptr, 12);
static AceButton b13(nullptr, 13);
static AceButton b14(nullptr, 14);
static AceButton b15(nullptr, 15);
static AceButton* const BUTTONS[] = {
    &b01, &b02, &b03, &b04, &b05, &b06, &b07,
    &b08, &b09, &b10, &b11, &b12, &b13, &b14, &b15,
};

// The EncodedButtonConfig constructor binds the AceButton to the
// EncodedButtonConfig.
static EncodedButtonConfig buttonConfig(NUM_PINS, PINS, NUM_BUTTONS, BUTTONS);

// Forward reference to prevent Arduino compiler becoming confused.
void handleEvent(AceButton*, uint8_t, uint8_t);

void setup() {
  delay(1000); // some microcontrollers reboot twice
  Serial.begin(115200);
  while (! Serial); // Wait until Serial is ready - Leonardo/Micro
  Serial.println(F("setup(): begin"));

  // Initialize built-in LED as an output.
  pinMode(LED_PIN, OUTPUT);

  // Pins use the built-in pull up register.
  for (uint8_t i = 0; i < NUM_PINS; i++) {
    pinMode(PINS[i], INPUT_PULLUP);
  }

  // Configure the ButtonConfig with the event handler, and enable all higher
  // level events.
  buttonConfig.setEventHandler(handleEvent);
  buttonConfig.setFeature(ButtonConfig::kFeatureClick);
  buttonConfig.setFeature(ButtonConfig::kFeatureDoubleClick);
  buttonConfig.setFeature(ButtonConfig::kFeatureLongPress);
  buttonConfig.setFeature(ButtonConfig::kFeatureRepeatPress);

  Serial.println(F("setup(): ready"));
}

void loop() {
  // Should be called every 4-5ms or faster, for the default debouncing time
  // of ~20ms.
  buttonConfig.checkButtons();
}

// The event handler for the buttons.
void handleEvent(AceButton* button, uint8_t eventType, uint8_t buttonState) {

  // Print out a message for all events.
  Serial.print(F("handleEvent(): "));
  Serial.print(F("virtualPin: "));
  Serial.print(button->getPin());
  Serial.print(F("; eventType: "));
  Serial.print(eventType);
  Serial.print(F("; buttonState: "));
  Serial.println(buttonState);

  // Control the LED only for the Pressed and Released events.
  // Notice that if the MCU is rebooted while the button is pressed down, no
  // event is triggered and the LED remains off.
  switch (eventType) {
    case AceButton::kEventPressed:
      digitalWrite(LED_PIN, LED_ON);
      break;
    case AceButton::kEventReleased:
      digitalWrite(LED_PIN, LED_OFF);
      break;
  }
}
