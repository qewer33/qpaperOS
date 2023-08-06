/*
 * A demo of 2 AceButtons using 2 ButtonConfigs.
 */

#include <AceButton.h>
using namespace ace_button;

// The pin number attached to the button.
const int BUTTON1_PIN = 2;
const int BUTTON2_PIN = 3;

#ifdef ESP32
  // Different ESP32 boards use different pins
  const int LED_PIN = 2;
#else
  const int LED_PIN = LED_BUILTIN;
#endif

// LED states. Some microcontrollers wire their built-in LED the reverse.
const int LED_ON = HIGH;
const int LED_OFF = LOW;

// Create 2 ButtonConfigs. The System ButtonConfig is not used.
ButtonConfig config1;
ButtonConfig config2;

// Two buttons, explicitly bound to their respective ButtonConfig, instead
// of the default System ButtonConfig.
AceButton button1(&config1);
AceButton button2(&config2);

// Two event handlers.
void handleEvent1(AceButton*, uint8_t, uint8_t);
void handleEvent2(AceButton*, uint8_t, uint8_t);

void setup() {
  delay(1000); // some microcontrollers reboot twice
  Serial.begin(115200);
  while (! Serial); // Wait until Serial is ready - Leonardo/Micro
  Serial.println(F("setup(): begin"));

  // Initialize built-in LED as an output.
  pinMode(LED_PIN, OUTPUT);

  // Buttons use the built-in pull up register.
  pinMode(BUTTON1_PIN, INPUT_PULLUP);
  pinMode(BUTTON2_PIN, INPUT_PULLUP);

  // Initialize the buttons
  button1.init(BUTTON1_PIN);
  button2.init(BUTTON2_PIN);

  // Configure ButtonConfig 1 with the event handler, and enable all higher
  // level events.
  config1.setEventHandler(handleEvent1);
  config1.setFeature(ButtonConfig::kFeatureClick);
  config1.setFeature(ButtonConfig::kFeatureDoubleClick);
  config1.setFeature(ButtonConfig::kFeatureLongPress);
  config1.setFeature(ButtonConfig::kFeatureRepeatPress);

  // Configure ButtonConfig 2 with just Click.
  config2.setEventHandler(handleEvent2);
  config2.setFeature(ButtonConfig::kFeatureClick);
  config2.setClickDelay(250); // increase click delay from default 200 ms

  // Check if the button was pressed while booting
  if (button1.isPressedRaw()) {
    Serial.println(F("setup(): button 1 was pressed while booting"));
  }
  if (button2.isPressedRaw()) {
    Serial.println(F("setup(): button 2 was pressed while booting"));
  }

  Serial.println(F("setup(): ready"));
}

void loop() {
  // Should be called every 4-5ms or faster, for the default debouncing time
  // of ~20ms.
  button1.check();
  button2.check();
}

void handleEvent1(AceButton* button, uint8_t eventType, uint8_t buttonState) {

  // Print out a message for all events, for both buttons.
  Serial.print(F("handleEvent1(): pin: "));
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
      digitalWrite(LED_PIN, LED_ON);
      break;
    case AceButton::kEventReleased:
      digitalWrite(LED_PIN, LED_OFF);
      break;
  }
}

void handleEvent2(AceButton* button, uint8_t eventType, uint8_t buttonState) {

  // Print out a message for all events, for both buttons.
  Serial.print(F("handleEvent2(): pin: "));
  Serial.print(button->getPin());
  Serial.print(F("; eventType: "));
  Serial.print(eventType);
  Serial.print(F("; buttonState: "));
  Serial.println(buttonState);

  // Control the LED only for the Pressed and Released events of Button 1.
  // Notice that if the MCU is rebooted while the button is pressed down, no
  // event is triggered and the LED remains off.
  switch (eventType) {
    case AceButton::kEventClicked:
      Serial.println(F("Button 2 clicked!"));
      break;
  }
}
