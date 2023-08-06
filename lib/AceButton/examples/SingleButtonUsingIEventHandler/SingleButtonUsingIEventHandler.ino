/*
 * A demo of a single AceButton which handles events using the IEventHandler
 * object, instead of the EventHandler function. Similar to SingleButton.ino.
 */

#include <AceButton.h>
using namespace ace_button;

// The pin number attached to the button.
const int BUTTON_PIN = 2;

#ifdef ESP32
  // Different ESP32 boards use different pins
  const int LED_PIN = 2;
#else
  const int LED_PIN = LED_BUILTIN;
#endif

// LED states. Some microcontrollers wire their built-in LED the reverse.
const int LED_ON = HIGH;
const int LED_OFF = LOW;

// One button wired to the pin at BUTTON_PIN. Automatically uses the default
// ButtonConfig. The alternative is to call the AceButton::init() method in
// setup() below.
AceButton button(BUTTON_PIN);

// An object that handles the event.
class ButtonHandler: public IEventHandler {
  public:
    void handleEvent(AceButton* /* button */, uint8_t eventType,
        uint8_t buttonState) override {

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
          digitalWrite(LED_PIN, LED_ON);
          break;
        case AceButton::kEventReleased:
          digitalWrite(LED_PIN, LED_OFF);
          break;
      }
    }
};

ButtonHandler handleEvent;

void setup() {
  delay(1000); // some microcontrollers reboot twice
  Serial.begin(115200);
  while (! Serial); // Wait until Serial is ready - Leonardo/Micro
  Serial.println(F("setup(): begin"));

  // initialize built-in LED as an output
  pinMode(LED_PIN, OUTPUT);

  // Button uses the built-in pull up register.
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Configure the ButtonConfig with the event handler, and enable all higher
  // level events.
  ButtonConfig* buttonConfig = button.getButtonConfig();
  buttonConfig->setIEventHandler(&handleEvent);
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

