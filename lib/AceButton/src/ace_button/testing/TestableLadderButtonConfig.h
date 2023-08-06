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

#ifndef ACE_BUTTON_TESTABLE_LADDER_BUTTON_CONFIG_H
#define ACE_BUTTON_TESTABLE_LADDER_BUTTON_CONFIG_H

#include "../LadderButtonConfig.h"

namespace ace_button {
namespace testing {

/**
 * A subclass of LadderButtonConfig which overrides getClock() and
 * getVirtualPin() so that their values can be controlled manually. This is
 * intended to be used for unit testing.
 */
class TestableLadderButtonConfig: public LadderButtonConfig {
  public:
    TestableLadderButtonConfig(
      uint8_t pin, uint8_t numLevels, const uint16_t levels[],
      uint8_t numButtons, AceButton* const buttons[],
      uint8_t defaultReleasedState = HIGH
    ):
      LadderButtonConfig(
        pin, numLevels, levels, numButtons, buttons, defaultReleasedState
      ),
      mMillis(0),
      mVirtualPin(0) {}

    /**
     * Initialize to its pristine state. This method is needed because AUnit
     * does not create a new instance of the Test class for each test case, so
     * we have to reuse objects between test cases, so we need a way to
     * reinitialize this object to its pristine state just after construction.
     */
    void init() {
      resetFeatures();
      mMillis = 0;
      mVirtualPin = 0;
    }

    unsigned long getClock() override { return mMillis; }

    uint8_t getVirtualPin() const override { return mVirtualPin; }

    /** Set the time of the fake clock. */
    void setClock(unsigned long millis) { mMillis = millis; }

    /** Set the virtual pin number. (nLevels-1) means "no button pressed". */
    void setVirtualPin(uint8_t pin) { mVirtualPin = pin; }

  private:
    // Disable copy-constructor and assignment operator
    TestableLadderButtonConfig(const TestableLadderButtonConfig&) = delete;
    TestableLadderButtonConfig& operator=(const TestableLadderButtonConfig&)
      = delete;

    unsigned long mMillis;
    uint8_t mVirtualPin;
};

}
}
#endif
