/*
 * Same as examples/SingleButton but using ButtonConfigFast1<PIN> instead of
 * ButtonConfig. Using the digitalWriteFast.h library saves 450 bytes (3608 ->
 * 3158) on an ATmega328 and 146 bytes (2722 -> 2576) on an ATtiny85.
 */

#include <Arduino.h>
#include <AceButton.h>
#include <digitalWriteFast.h>
#include <ace_button/fast/ButtonConfigFast1.h>

using namespace ace_button;

// The physical pin number attached to the button.
const int BUTTON_PHYSICAL_PIN = 2;

// The virtual pin number attached to the button.
const uint8_t BUTTON_PIN = 0;

#ifdef ESP32
  // Different ESP32 boards use different pins
  const int LED_PIN = 2;
#else
  const int LED_PIN = LED_BUILTIN;
#endif

// LED states. Some microcontrollers wire their built-in LED the reverse.
const int LED_ON = HIGH;
const int LED_OFF = LOW;

// Use the custom ButtonConfigFast1<> instance instead of the system
// ButtonConfig.
ButtonConfigFast1<BUTTON_PHYSICAL_PIN> buttonConfig;
AceButton button(&buttonConfig, BUTTON_PIN);

// Forward reference to prevent Arduino compiler becoming confused.
void handleEvent(AceButton*, uint8_t, uint8_t);

void setup() {
  delay(1000); // some microcontrollers reboot twice
  Serial.begin(115200);
  while (! Serial); // Wait until Serial is ready - Leonardo/Micro
  Serial.println(F("setup(): begin"));

  // initialize built-in LED as an output
  pinModeFast(LED_PIN, OUTPUT);
  digitalWriteFast(LED_PIN, LED_OFF);

  // Button uses the built-in pull up register.
  pinModeFast(BUTTON_PIN, INPUT_PULLUP);

  // Configure the ButtonConfig with the event handler, and enable all higher
  // level events.
  ButtonConfig* buttonConfig = button.getButtonConfig();
  buttonConfig->setEventHandler(handleEvent);
  buttonConfig->setFeature(ButtonConfig::kFeatureClick);
  buttonConfig->setFeature(ButtonConfig::kFeatureDoubleClick);
  buttonConfig->setFeature(ButtonConfig::kFeatureLongPress);
  buttonConfig->setFeature(ButtonConfig::kFeatureRepeatPress);

  // Check if the button was pressed while booting
  if (button.isPressedRaw()) {
    Serial.println(F("setup(): button was pressed while booting"));
  }

  Serial.println(F("setup(): ready"));
}

void loop() {
  // Should be called every 4-5ms or faster, for the default debouncing time
  // of ~20ms.
  button.check();
}

// The event handler for the button.
void handleEvent(AceButton* /* button */, uint8_t eventType,
    uint8_t buttonState) {

  // Print out a message for all events.
  Serial.print(F("handleEvent(): eventType: "));
  Serial.print(eventType);
  Serial.print(F("; buttonState: "));
  Serial.println(buttonState);

  // Control the LED only for the Pressed and Released events.
  // Notice that if the MCU is rebooted while the button is pressed down, no
  // event is triggered and the LED remains off.
  switch (eventType) {
    case AceButton::kEventPressed:
      digitalWriteFast(LED_PIN, LED_ON);
      break;
    case AceButton::kEventReleased:
      digitalWriteFast(LED_PIN, LED_OFF);
      break;
  }
}
