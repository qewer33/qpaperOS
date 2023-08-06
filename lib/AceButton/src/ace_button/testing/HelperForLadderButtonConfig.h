/*
MIT License

Copyright (c) 2018 Brian T. Park

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

#ifndef ACE_BUTTON_HELPER_FOR_LADDER_BUTTON_CONFIG_H
#define ACE_BUTTON_HELPER_FOR_LADDER_BUTTON_CONFIG_H

#include <AceButton.h>
#include <ace_button/testing/TestableLadderButtonConfig.h>
#include <ace_button/testing/EventTracker.h>

namespace ace_button {
namespace testing {

/**
 * A wrapper class around a LadderButtonConfig that sends emulated button
 * presses and released to the underlying AceButton class, and captures the
 * resulting events in the provided EventTracker.
 */
class HelperForLadderButtonConfig {
  public:
    HelperForLadderButtonConfig(
        TestableLadderButtonConfig* testableConfig,
        EventTracker* eventTracker):
      mTestableConfig(testableConfig),
      mEventTracker(eventTracker) {}

    /** Reinitilize to its pristine state. */
    void init() {
      mEventTracker->clear();
      mTestableConfig->init();
    }

    /**
     * Simulate a press of the button and run the button.check() processing. The
     * defaultReleasedState is determined by whether the button has a pullup
     * (HIGH) or pulldown (LOW) resistor.
     */
    void pressButton(unsigned long time, uint8_t virtualPin) {
      mTestableConfig->setClock(time);
      mTestableConfig->setVirtualPin(virtualPin);
      mEventTracker->clear();
      mTestableConfig->checkButtons();
    }

    /**
     * Simulate a release of the button and run the button.check() processing.
     */
    void releaseButton(unsigned long time) {
      mTestableConfig->setClock(time);
      mTestableConfig->setVirtualPin(mTestableConfig->getNoButtonPin());
      mEventTracker->clear();
      mTestableConfig->checkButtons();
    }

    /**
     * Simply move the time forward and check the button.
     */
    void checkTime(unsigned long time) {
      mTestableConfig->setClock(time);
      mEventTracker->clear();
      mTestableConfig->checkButtons();
    }

  private:
    // Disable copy-constructor and assignment operator
    HelperForLadderButtonConfig(const HelperForLadderButtonConfig&) = delete;
    HelperForLadderButtonConfig&
        operator=(const HelperForLadderButtonConfig&) = delete;

    TestableLadderButtonConfig* mTestableConfig;
    EventTracker* mEventTracker;
};

}
}
#endif
