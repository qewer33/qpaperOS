/*
 * A demo of 3 AceButtons using a single ButtonConfigFast3. Compared to
 * ThreeButtonsUsingOneButtonConfig, this saves about 424 bytes (3866 -> 3442)
 * of flash on an ATmega328, and 228 bytes (3066 -> 2838) on an ATtiny85.
 */

#include <AceButton.h>
#include <digitalWriteFast.h>
#include <ace_button/fast/ButtonConfigFast3.h>
using namespace ace_button;

// Physical pin numbers attached to the buttons.
const uint8_t BUTTON1_PHYSICAL_PIN = 2;
const uint8_t BUTTON2_PHYSICAL_PIN = 3;
const uint8_t BUTTON3_PHYSICAL_PIN = 4;

// Virtual pin numbers attached to the buttons.
const uint8_t BUTTON1_PIN = 0;
const uint8_t BUTTON2_PIN = 1;
const uint8_t BUTTON3_PIN = 2;

#ifdef ESP32
  // Different ESP32 boards use different pins
  const int LED_PIN = 2;
#else
  const int LED_PIN = LED_BUILTIN;
#endif

// LED states. Some microcontrollers wire their built-in LED the reverse.
const int LED_ON = HIGH;
const int LED_OFF = LOW;

// All buttons automatically use the default System ButtonConfig. The
// alternative is to call the AceButton::init() method in setup() below.
ButtonConfigFast3<
    BUTTON1_PHYSICAL_PIN,
    BUTTON2_PHYSICAL_PIN,
    BUTTON3_PHYSICAL_PIN
> buttonConfig;
AceButton button1(&buttonConfig, BUTTON1_PIN);
AceButton button2(&buttonConfig, BUTTON2_PIN);
AceButton button3(&buttonConfig, BUTTON3_PIN);

// Forward reference to prevent Arduino compiler becoming confused.
void handleEvent(AceButton*, uint8_t, uint8_t);

void setup() {
  delay(1000); // some microcontrollers reboot twice
  Serial.begin(115200);
  while (! Serial); // Wait until Serial is ready - Leonardo/Micro
  Serial.println(F("setup(): begin"));

  // Initialize built-in LED as an output.
  pinModeFast(LED_PIN, OUTPUT);

  // Buttons use the built-in pull up register.
  pinModeFast(BUTTON1_PIN, INPUT_PULLUP);
  pinModeFast(BUTTON2_PIN, INPUT_PULLUP);
  pinModeFast(BUTTON3_PIN, INPUT_PULLUP);

  // Configure the ButtonConfig with the event handler, and enable all higher
  // level events.
  buttonConfig.setEventHandler(handleEvent);
  buttonConfig.setFeature(ButtonConfig::kFeatureClick);
  buttonConfig.setFeature(ButtonConfig::kFeatureDoubleClick);
  buttonConfig.setFeature(ButtonConfig::kFeatureLongPress);
  buttonConfig.setFeature(ButtonConfig::kFeatureRepeatPress);

  // Check if the button was pressed while booting
  if (button1.isPressedRaw()) {
    Serial.println(F("setup(): button 1 was pressed while booting"));
  }
  if (button2.isPressedRaw()) {
    Serial.println(F("setup(): button 2 was pressed while booting"));
  }
  if (button3.isPressedRaw()) {
    Serial.println(F("setup(): button 3 was pressed while booting"));
  }

  Serial.println(F("setup(): ready"));
}

void loop() {
  // Should be called every 4-5ms or faster, for the default debouncing time
  // of ~20ms.
  button1.check();
  button2.check();
  button3.check();
}

// The event handler for both buttons.
void handleEvent(AceButton* button, uint8_t eventType, uint8_t buttonState) {

  // Print out a message for all events, for both buttons.
  Serial.print(F("handleEvent(): pin: "));
  Serial.print(button->getPin());
  Serial.print(F("; eventType: "));
  Serial.print(eventType);
  Serial.print(F("; buttonState: "));
  Serial.println(buttonState);

  // Control the LED only for the Pressed and Released events of Button 1.
  // Notice that if the MCU is rebooted while the button is pressed down, no
  // event is triggered and the LED remains off.
  switch (eventType) {
    case AceButton::kEventPressed:
      if (button->getPin() == BUTTON1_PIN) {
        digitalWriteFast(LED_PIN, LED_ON);
      }
      break;
    case AceButton::kEventReleased:
      if (button->getPin() == BUTTON1_PIN) {
        digitalWriteFast(LED_PIN, LED_OFF);
      }
      break;
    case AceButton::kEventClicked:
      if (button->getPin() == BUTTON2_PIN) {
        Serial.println(F("Button 2 clicked!"));
      } else if (button->getPin() == BUTTON3_PIN) {
        Serial.println(F("Button 3 clicked!"));
      }
      break;
  }
}
