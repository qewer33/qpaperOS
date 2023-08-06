/*
MIT License

Copyright (c) 2020 Brian T. Park

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef ACE_BUTTON_LADDER_BUTTON_CONFIG_H
#define ACE_BUTTON_LADDER_BUTTON_CONFIG_H

#include "ButtonConfig.h"

// Unit test
class LadderButtonConfig_extractIndex;

namespace ace_button {

class AceButton;

/**
 * A ButtonConfig that handles multiple buttons using a resistor ladder.
 */
class LadderButtonConfig : public ButtonConfig {
  public:

    /**
     * Constructor.
     * @param pin the analog pin where the buttons and resistors are connected
     * @param numLevels number of voltage levels from the ADC, must be greater
     *        than numButtons
     * @param levels an array of expected outputs of the ADC at each level.
     *        These values must be monotonically increasing. level[0] will
     *        probably be 0, corresponding to button[0] pulling down to 0V, and
     *        level[numLevels-1] is the maximum value of the ADC, corresponding
     *        to no-button pressed. It will be 1023 for a 10-bit ADC, and
     *        4095 for a 12-bit ADC.
     * @param numButtons number buttons on the ladder
     * @param buttons array of AceButton instances which are attached to the
     *        ladder
     * @param defaultReleasedState state of the encoder bit when the button
     *        is in the released state. For a pull-up wiring, the state of the
     *        pin is HIGH when the button is released.
     */
    LadderButtonConfig(uint8_t pin, uint8_t numLevels, const uint16_t levels[],
        uint8_t numButtons, AceButton* const buttons[],
        uint8_t defaultReleasedState = HIGH);

    /**
     * Return state of the button corresponding to the virtual 'pin' number.
     * LOW means that the corresponding encoded virtual pin was pushed.
     *
     * This method is not expected to be used. Use the checkButtons() method
     * instead for this class.
     */
    int readButton(uint8_t pin) override;

    /**
     * Read the single mPin once, calculate the virtual pin number of the
     * pressed button (if any), then call each button's checkState() method to
     * trigger any events. This is more efficient than calling the check() of
     * each AceButton, which in turn, calls LadderButtonConfig::readButton() for
     * each button.
     *
     * Warning: This method is often called from the global `loop()` function.
     * But on ESP8266, this method must not be called too quickly, otherwise it
     * triggers a bug which causes the WiFi to disconnect (see
     * https://github.com/esp8266/Arduino/issues/1634 and
     * https://github.com/esp8266/Arduino/issues/5083). You must rate-limit the
     * calls to this method to ~5 milliseconds. See
     * `examples/LadderButtons/LadderButtons.ino` for an example of how to do
     * that.
     */
    void checkButtons() const;

    /** The virtual button pin number corresponding to "no button" pressed. */
    uint8_t getNoButtonPin() const {
      return mNumLevels - 1;
    }

  protected:
    /**
     * Return the virtual pin number corresponding to current state of the ADC
     * as returned by the `analogRead()` function. When no button is pressed,
     * this returns (numLevels - 1), which does not correspond to any valid
     * button.
     */
    virtual uint8_t getVirtualPin() const;

  private:
    // Allow unit test to access extractIndex().
    friend class ::LadderButtonConfig_extractIndex;

    // Disable copy-constructor and assignment operator
    LadderButtonConfig(const LadderButtonConfig&) = delete;
    LadderButtonConfig& operator=(const LadderButtonConfig&) = delete;

    /**
     * Return the index of 'levels[]' which matches the given 'level'. Extracted
     * as a static function for unit testing.
     */
    static uint8_t extractIndex(uint8_t numLevels, uint16_t const levels[],
        uint16_t level);

  private:
    // Arranged for efficient packing on 32-bit processors
    uint8_t const mPin;
    uint8_t const mNumLevels;
    uint8_t const mNumButtons;
    uint8_t const mPressedState;
    uint16_t const* const mLevels;
    AceButton* const* const mButtons;
};

}

#endif
