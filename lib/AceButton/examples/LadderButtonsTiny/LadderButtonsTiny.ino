/*
 * A version of LadderButtons specially crafted for an ATtiny85 with 2 buttons
 * connected to the one of the analog pins using a resistor ladder. The ATtiny85
 * does not have a Serial port, so we use a single LED connected to the D0 pin.
 *
 * When no button is pressed, this program emits one short blink every 1.5
 * seconds. When Button 0 is pressed, it emits two short blinks every 1.5
 * seconds. When Button 1 is pressed, it emits 3 short blinks every 1.5 seconds.
 *
 * The ATtiny85 has only 6 GPIO pins which makes them very precious. One of the
 * GPIO pins does double duty as the RESET pin. It turns out that it is possible
 * to use the RESET pin as an analog input pin, as long as the signal remains
 * above the RESET threshold voltage. This saves the other 5 pins for other
 * purposes.
 *
 * The ATtiny85 datasheet says that V(RST) can vary from 0.2 VCC to 0.9 VCC.
 * Let's keep the voltage level above 90%. (On my ATtiny85, I applied a level as
 * low as 50% VCC and my microcontroller still did NOT reset, so I guess it is
 * closer to the 0.2 VCC than the 0.9 VCC).
 *
 * If the analog to digital converter is stable (with respect to temperature and
 * time), then we should be able to connect 2 buttons (S0, S1) through a
 * resistor ladder. The schematic looks something like this:
 *
 *       ATtiny85
 *     +-----------+
 *   +-|RST/A0  VCC|---------------+
 *   | |D3/A3 A1/D2|               |
 *   | |D4/D2    D1|               |
 *   | |GND      D0|-- R -- LED    |
 *   | +-----------+         |     |
 *   |                      GND    |
 *   |                             |
 *   |               +-------------+
 *   |               |
 *   |              1k
 *   |               |
 *   +-----------+---+---+
 *               |       |
 *              10k     22k
 *               |       |
 *               *       *
 *              /       /
 *             v  S0   v  S1
 *               *       *
 *               |       |
 *               +-------+
 *                   |
 *                  GND
 *
 * The computed voltage levels of S0 and S1 are 90.9% (10k/11k) and 95.7%
 * (22k/23k) respectively. Since the ATtiny85 has a 10-bit ADC, the highest
 * value, with an open circuit (no switch pressed), is 1023. So the analogRead()
 * values are 930 and 979. Using examples/LadderButtonCalibrator, we find that
 * actual values are 933 and 980, which are very close to the theoretical
 * predictions. These are the values used in the LEVELS[] array below.
 *
 * The ATtiny85 does not have a USB-Serial converter, so we cannot simply print
 * to the Serial object. The simplest way to send feedback about which button
 * was pressed is through an LED connected to one of the pins. This program
 * assumes that such an LED is connect to one of the digital output pins.
 *
 * DigitalWriteFast: I tried converting this program to use the digitalWriteFast
 * library. LadderButtonConfig uses analogRead() which is not supported by
 * digitalWriteFast. We can convert the digitalWrite() and pinMode() calls below
 * to digitalWriteFast() and pinModeFast(). The difference is only about 50
 * bytes of flash, so this conversion does not seem worth the effort.
 */

#include <stdint.h>
#include <Arduino.h>
#include <AceButton.h>
using ace_button::AceButton;
using ace_button::ButtonConfig;
using ace_button::LadderButtonConfig;

//-----------------------------------------------------------------------------
// Configure external LED
//-----------------------------------------------------------------------------

// Pin of the external LED.
static const int LED_PIN = 0;

// LED on and off values, depending on it was wired.
static const int LED_ON = HIGH;
static const int LED_OFF = LOW;

void blink() {
  digitalWrite(LED_PIN, LED_ON);
  delay(100);
  digitalWrite(LED_PIN, LED_OFF);
  delay(100);
}

uint8_t blinkCount = 1;

void blinkLed() {
  static uint16_t prev;

  uint16_t now = millis();
  if (now - prev >= 1500) {
    prev = now;
    if (blinkCount == 1) {
      blink();
    } else if (blinkCount == 2) {
      blink();
      blink();
    } else if (blinkCount == 3) {
      blink();
      blink();
      blink();
    }
  }
}

//-----------------------------------------------------------------------------
// Configure AceButton
//-----------------------------------------------------------------------------

// The ADC pin used by the resistor ladder.
static const uint8_t BUTTON_PIN = A0; // RESET pin

// Create 2 AceButton objects, with their virtual pin numbers 0 and 1.
//
// We use the 4-parameter AceButton() constructor with the `buttonConfig`
// parameter set to `nullptr` to prevent the creation of the default
// SystemButtonConfig which will never be used. This saves about 30 bytes of
// flash and 26 bytes of static RAM on an AVR processor.
static const uint8_t NUM_BUTTONS = 2;
static AceButton b0(nullptr, 0);
static AceButton b1(nullptr, 1);
static AceButton* const BUTTONS[NUM_BUTTONS] = { &b0, &b1 };

// Define the ADC voltage levels for each button. For 2 buttons, we need 3
// levels. The values were calculated using the voltage divider formula, then
// validated using examples/LadderButtonCalibrator. These buttons are connected
// to the RESET button on the ATtiny85, so they must be greater than 90% of VCC
// to prevent reset.
static const uint8_t NUM_LEVELS = NUM_BUTTONS + 1;
static const uint16_t LEVELS[NUM_LEVELS] = {
  933 /* 91.2%, 10 kohm */,
  980 /* 95.8%, 22 kohm */,
  1023 /* 100%, open circuit */,
};

// The LadderButtonConfig constructor binds the AceButton objects in the BUTTONS
// array to the LadderButtonConfig.
static LadderButtonConfig buttonConfig(
  BUTTON_PIN, NUM_LEVELS, LEVELS, NUM_BUTTONS, BUTTONS
);

// The event handler for the buttons.
void handleEvent(AceButton* button, uint8_t eventType,
    uint8_t /*buttonState*/) {

  // Control the LED only for the Pressed and Released events.
  // Notice that if the MCU is rebooted while the button is pressed down, no
  // event is triggered and the LED remains off.
  switch (eventType) {
    case AceButton::kEventPressed: {
      uint8_t pin = button->getPin();
      if (pin == 0) {
        blinkCount = 2;
      } else if (pin == 1) {
        blinkCount = 3;
      }
      break;
    }
    case AceButton::kEventReleased:
      blinkCount = 1;
      break;
  }
}

// On most processors, this should be called every 4-5ms or faster, if default
// the debouncing time is ~20ms. On a ESP8266, we must sample *no* faster than
// 4-5 ms to avoid disconnecting the WiFi connection. (See
// https://github.com/esp8266/Arduino/issues/1634 and
// https://github.com/esp8266/Arduino/issues/5083). Let's rate-limit on all
// processors to about 200 samples/second.
void checkButtons() {
  static uint16_t prev = millis();

  // DO NOT USE delay(5) to do this.
  uint16_t now = millis();
  if (now - prev >= 5) {
    prev = now;
    buttonConfig.checkButtons();
  }
}

//-----------------------------------------------------------------------------

void setup() {
  delay(1000); // some microcontrollers reboot twice

  // initialize built-in LED as an output
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LED_OFF);

  // Don't use internal pull-up resistor.
  pinMode(BUTTON_PIN, INPUT);

  // Configure the ButtonConfig with the event handler, and enable all higher
  // level events.
  buttonConfig.setEventHandler(handleEvent);
  buttonConfig.setFeature(ButtonConfig::kFeatureClick);
  buttonConfig.setFeature(ButtonConfig::kFeatureDoubleClick);
  buttonConfig.setFeature(ButtonConfig::kFeatureLongPress);
  buttonConfig.setFeature(ButtonConfig::kFeatureRepeatPress);
}

void loop() {
  checkButtons();
  blinkLed();
}
