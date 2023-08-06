/*
 * A simple loop to print out the value of the analog pin when buttons are
 * pressed. The buttons are connected to a single pin using a resistor ladder.
 * See examples/LadderButtons to see how to configure the LadderButtonConfig to
 * detect button events.
 */

#include <stdint.h>
#include <Arduino.h>

//-----------------------------------------------------------------------------

// Choose the output destination. Normally, we would send to the Serial monitor.
// But some environments (e.g. ATtiny85) do not normally provide a USB-to-Serial
// monitor. One alternative would be to use a seven-segment LED module and print
// the analogRead() value to the LED module. This option requires the AceSegment
// (https://github.com/bxparks/AceSegment) library. The example below uses a
// TM1637 LED module.
#define OUTPUT_TYPE_SERIAL 0
#define OUTPUT_TYPE_TM1637 1
#define OUTPUT_TYPE OUTPUT_TYPE_SERIAL

#if OUTPUT_TYPE == OUTPUT_TYPE_TM1637
  #include <AceSegment.h>
  using ace_segment::SoftWireInterface;
  using ace_segment::Tm1637Module;
  using ace_segment::LedDisplay;
  using ace_segment::NumberWriter;

  // These pins are for my ATtiny85 board.
  const uint8_t CLK_PIN = 3;
  const uint8_t DIO_PIN = 1;
  const uint16_t BIT_DELAY = 100;
  const uint8_t NUM_DIGITS = 4;

  using WireInterface = SoftWireInterface;
  WireInterface wireInterface(CLK_PIN, DIO_PIN, BIT_DELAY);
  Tm1637Module<WireInterface, NUM_DIGITS> tm1637Module(wireInterface);
  LedDisplay ledDisplay(tm1637Module);
  NumberWriter numberWriter (ledDisplay);

  void flushLed() {
    static uint16_t prevMillis;

    uint16_t nowMillis = millis();
    if (nowMillis - prevMillis >= 100) {
      prevMillis = nowMillis;
      tm1637Module.flush();
    }
  }

#endif

//-----------------------------------------------------------------------------

// Define the actual ADC pin.
static const uint8_t BUTTON_PIN = A0;

// Call analogRead() every 5 ms (which is how fast
// LadderButtonConfig::checkButtons() should be called), and just print the
// value. Click on the various buttons to see the actual value is returned by
// each button.
void calibrateAnalogRead() {
  static uint16_t prevMillis;

  uint16_t nowMillis = millis();
  if (nowMillis - prevMillis >= 5) {
    prevMillis = nowMillis;

    uint16_t val = analogRead(BUTTON_PIN);

  #if OUTPUT_TYPE == OUTPUT_TYPE_SERIAL
    Serial.println(val);
  #elif OUTPUT_TYPE == OUTPUT_TYPE_TM1637
    numberWriter.writeUnsignedDecimalAt(0, val, 4 /*boxSize*/);
  #endif

  }
}

//-----------------------------------------------------------------------------

void setup() {
  delay(1000); // some microcontrollers reboot twice

#if OUTPUT_TYPE == OUTPUT_TYPE_SERIAL
  Serial.begin(115200);
  while (! Serial); // Wait until Serial is ready - Leonardo/Micro
  Serial.println(F("setup(): begin"));

#elif OUTPUT_TYPE == OUTPUT_TYPE_TM1637
  wireInterface.begin();
  tm1637Module.begin();
  ledDisplay.setBrightness(3);
#endif

  // Don't use internal pull-up resistor because it will change the effective
  // resistance of the resistor ladder.
  pinMode(BUTTON_PIN, INPUT);

#if OUTPUT_TYPE == OUTPUT_TYPE_SERIAL
  Serial.println(F("setup(): ready"));
#endif
}

void loop() {
  calibrateAnalogRead();

#if OUTPUT_TYPE == OUTPUT_TYPE_TM1637
  flushLed();
#endif

}
