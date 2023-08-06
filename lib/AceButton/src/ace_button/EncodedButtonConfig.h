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

#ifndef ACE_BUTTON_ENCODED_BUTTON_CONFIG_H
#define ACE_BUTTON_ENCODED_BUTTON_CONFIG_H

#include "ButtonConfig.h"

namespace ace_button {

class AceButton;

/**
 * A ButtonConfig that handles an N-to-M binary encoder where N = 2^M. This is a
 * generalized version of Encoded4To2ButtonConfig and Encoded8To3ButtonConfig.
 *
 * Here is an example of how to make `EncodedButtonConfig` act like an
 * `Encoded8To3ButtonConfig`:
 *
 * @code
 * static const uint8_t PINS[] = {2, 4, 5};
 *
 * static AceButton b1(1);
 * static AceButton b2(2);
 * static AceButton b3(3);
 * static AceButton b5(5);
 * static AceButton b6(6);
 * static AceButton* const BUTTONS[] = {&b1, &b2, &b3, &b5, &b6};
 *
 * static EncodedButtonConfig buttonConfig(3, PINS, 5, BUTTONS);
 *
 * void handleEvent(AceButton*, uint8_t, uint8_t);
 *
 * void setup() {
 *   pinMode(2, INPUT_PULLUP);
 *   pinMode(4, INPUT_PULLUP);
 *   pinMode(5, INPUT_PULLUP);
 *
 *   buttonConfig.setEventHandler(handleEvent);
 *   ...
 * }
 *
 * void loop() {
 *   buttonConfig.checkButtons();
 *   ...
 * }
 *
 * @endcode
 */
class EncodedButtonConfig : public ButtonConfig {
  public:

    /**
     * Constructor.
     * @param numPins number of pins used to encode the switches
     *        (corresponding to the M)
     * @param pins an array of actual pin numbers (e.g. [2, 4, 5])
     * @param numButtons number buttons which are encoded; the maximum number
     *        of buttons is 2^{numPins}
     * @param buttons array of buttons attached to the pins using binary
     *        encoding; each button needs to be assigned a virtual pin number
     *        between 1 and (2^{numPins} - 1) inclusive; the buttons can be
     *        arranged in any order
     * @param defaultReleasedState state of the encoder bit when the button
     *        is in the released state. For a pull-up wiring, the state of the
     *        pin is HIGH when the button is released. This value is used to
     *        configure wiring of the virtual button, so that it matches the
     *        wiring of the physical buttons. The LS74148 encoder uses a pull-up
     *        wiring, so this should be set HIGH. The default value is HIGH.
     */
    EncodedButtonConfig(uint8_t numPins, const uint8_t pins[],
        uint8_t numButtons, AceButton* const buttons[],
        uint8_t defaultReleasedState = HIGH);

    /**
     * Return state of the virtual (i.e. encoded) 'pin' number, corresponding to
     * the pull-down states of the actual pins. LOW means that the corresponding
     * encoded virtual pin was pushed.
     *
     * This method is not expected to be used. Use the checkButtons() method
     * instead for this class.
     */
    int readButton(uint8_t pin) override;

    /**
     * Read the pins once, obtain the virtual pin number, then call each
     * button's checkState() method to trigger any events. When the number of
     * buttons becomes greater than 7 or 8, it is more efficient to call this
     * method, instead of calling the check() of each AceButton.
     */
    void checkButtons() const;

    /** The virtual button pin number corresponding to "no button" pressed. */
    uint8_t getNoButtonPin() const {
      return 0;
    }

  protected:
    /**
     * Return the virtual pin number corresponding to the combinatorial states
     * of the actual pins. Returns a number between 1 and (2^{numPins} - 1). 0
     * means "no button" pressed.
     */
    virtual uint8_t getVirtualPin() const;

  private:
    // Disable copy-constructor and assignment operator
    EncodedButtonConfig(const EncodedButtonConfig&) = delete;
    EncodedButtonConfig& operator=(const EncodedButtonConfig&) = delete;

  private:
    // Arranged for efficient packing on 32-bit processors
    uint8_t const mNumPins;
    uint8_t const mNumButtons;
    uint8_t const mPressedState;
    const uint8_t* const mPins;
    AceButton* const* const mButtons;
};

}

#endif
