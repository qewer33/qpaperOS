/*
 * A demo of two buttons that controls a counter, demonstrating 2 advanced
 * features:
 *
 *    * Using Released to distinguish a Pressed event from a LongPressed event
 *      (since buttons always generate a Pressed event).
 *    * Using a LongReleased event to detect the end of a LongPressed event.
 *
 * User Operation:
 *
 *    * The Mode button uses LongPressed event to switch in and out of Edit
 *      mode.
 *    * The Change button increments an internal counter, using a Pressed or
 *      RepeatPressed events. This button is active only in Edit mode.
 *
 * Expected Output should be something like this:
 *
 * @verbatim
 * Mode Button: Pressed
 * New state: 1
 * Mode Button: Pressed
 * New state: 2
 * Mode Button: Long Pressed
 * Editting: true; blinking: true
 * Change Button: Pressed
 * Counter: 1; blinking: false
 * Change Button: Released
 * Counter: 1; blinking: true
 * Change Button: Pressed
 * Counter: 2; blinking: false
 * Change Button: Repeat Pressed
 * Counter: 3; blinking: false
 * Change Button: Repeat Pressed
 * Counter: 4; blinking: false
 * Change Button: Repeat Pressed
 * Counter: 5; blinking: false
 * Change Button: Repeat Pressed
 * Counter: 6; blinking: false
 * Change Button: Repeat Pressed
 * Counter: 7; blinking: false
 * Change Button: Repeat Pressed
 * Counter: 8; blinking: false
 * Change Button: Repeat Pressed
 * Counter: 9; blinking: false
 * Change Button: Repeat Pressed
 * Counter: 10; blinking: false
 * Change Button: Released
 * Counter: 10; blinking: true
 * Mode Button: Long Pressed
 * Editting: false; blinking: true
 * @endverbatim
 */

#include <AceButton.h>
using namespace ace_button;

// The pin numbers attached to the buttons.
const int MODE_BUTTON_PIN = 2;
const int CHANGE_BUTTON_PIN = 3;

// Two buttons, automatically sharing the default System ButtonConfig.
AceButton modeButton(MODE_BUTTON_PIN);
AceButton changeButton(CHANGE_BUTTON_PIN);

// Three states: 0, 1, and 2.
int state = 0;

// Change button increments this counter in Edit mode.
int counter = 0;

// LongPress on ModeButton will go into "edit" mode.
bool isEditting = false;

// In edit mode, the "field" is blinking. But when the Change button is
// Pressed or LongPressed, the blinking temporarily stops.
bool isBlinking = false;

// The event handler for the buttons.
void handleEvent(AceButton* button, uint8_t eventType,
    uint8_t /*buttonState*/) {
  uint8_t pin = button->getPin();

  /*
  // Print out a message for all events.
  Serial.print(F("handleEvent(): pin: "));
  Serial.print(pin);
  Serial.print(F("; eventType: "));
  Serial.print(eventType);
  Serial.print(F("; buttonState: "));
  Serial.println(buttonState);
  */

  if (pin == MODE_BUTTON_PIN) {
    switch (eventType) {
      // Interpret a Released event as a Pressed event, to distiguish it
      // from a LongPressed event.
      case AceButton::kEventReleased:
        if (!isEditting) {
          state++;
          if (state > 2) state = 0;
          Serial.println(F("Mode Button: Pressed"));
          Serial.print(F("New state: "));
          Serial.println(state);
        }
        break;

      // LongPressed goes in and out of edit mode.
      case AceButton::kEventLongPressed:
        isEditting = !isEditting;
        if (isEditting) {
          isBlinking = true;
        }
        Serial.println(F("Mode Button: Long Pressed"));
        Serial.print(F("Editting: "));
        Serial.print(isEditting ? F("true") : F("false"));
        Serial.print(F("; blinking: "));
        Serial.println(isBlinking ? F("true") : F("false"));
        break;
    }
  } else if (pin == CHANGE_BUTTON_PIN) {
    switch (eventType) {
      case AceButton::kEventPressed:
      case AceButton::kEventRepeatPressed:
        isBlinking = false;
        if (isEditting) {
          if (eventType == AceButton::kEventPressed) {
            Serial.println(F("Change Button: Pressed"));
          } else {
            Serial.println(F("Change Button: Repeat Pressed"));
          }
          counter++;
          Serial.print(F("Counter: "));
          Serial.print(counter);
          Serial.print(F("; blinking: "));
          Serial.println(isBlinking ? F("true") : F("false"));
        }
        break;

      case AceButton::kEventReleased:
      case AceButton::kEventLongReleased:
        isBlinking = true;
        if (isEditting) {
          Serial.println(F("Change Button: Released"));
          Serial.print(F("Counter: "));
          Serial.print(counter);
          Serial.print(F("; blinking: "));
          Serial.println(isBlinking ? F("true") : F("false"));
        }
        break;
    }
  }
}

//----------------------------------------------------------------------------

void setup() {
  delay(1000); // some microcontrollers reboot twice
  Serial.begin(115200);
  while (! Serial); // Wait until Serial is ready - Leonardo/Micro
  Serial.println(F("setup(): begin"));

  // Button uses the built-in pull up register.
  pinMode(MODE_BUTTON_PIN, INPUT_PULLUP);
  pinMode(CHANGE_BUTTON_PIN, INPUT_PULLUP);

  // Configure the ButtonConfig with the event handler.
  ButtonConfig* buttonConfig = ButtonConfig::getSystemButtonConfig();
  buttonConfig->setEventHandler(handleEvent);
  buttonConfig->setFeature(ButtonConfig::kFeatureLongPress);
  buttonConfig->setFeature(ButtonConfig::kFeatureRepeatPress);
  buttonConfig->setFeature(ButtonConfig::kFeatureSuppressAfterLongPress);

  Serial.println(F("setup(): ready"));
}

void loop() {
  // Should be called every 4-5ms or faster, for the default debouncing time
  // of ~20ms.
  modeButton.check();
  changeButton.check();
}
