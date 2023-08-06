/*
 * A demo of Encoded4To2ButtonConfig to detect 3 buttons using 2 pins using
 * binary encoding.
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

// Physical pins
static const uint8_t BUTTON_PIN0 = 2;
static const uint8_t BUTTON_PIN1 = 3;

// Each button is assigned to the virtual pin number (1-3) which comes from the
// binary bit patterns of the 2 actual pins. Button b0 on virtual pin 0 cannot
// be used because it is used to represent "no button pressed".
Encoded4To2ButtonConfig buttonConfig(BUTTON_PIN0, BUTTON_PIN1);
AceButton b1(&buttonConfig, 1);
AceButton b2(&buttonConfig, 2);
AceButton b3(&buttonConfig, 3);

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
  pinMode(BUTTON_PIN0, INPUT_PULLUP);
  pinMode(BUTTON_PIN1, INPUT_PULLUP);

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
  b1.check();
  b2.check();
  b3.check();
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
