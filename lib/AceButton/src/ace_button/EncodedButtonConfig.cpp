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

#include "EncodedButtonConfig.h"
#include "AceButton.h"

namespace ace_button {

EncodedButtonConfig::EncodedButtonConfig(
      uint8_t numPins, const uint8_t pins[], uint8_t numButtons,
      AceButton* const buttons[], uint8_t defaultReleasedState):
    mNumPins(numPins),
    mNumButtons(numButtons),
    mPressedState(defaultReleasedState ^ 0x1),
    mPins(pins),
    mButtons(buttons) {
  for (uint8_t i = 0; i < mNumButtons; i++) {
    AceButton* button = mButtons[i];
    button->setButtonConfig(this);
  }
}

int EncodedButtonConfig::readButton(uint8_t pin) {
  uint8_t virtualPin = getVirtualPin();
  return (virtualPin == pin) ? mPressedState : (mPressedState ^ 0x1);
}

void EncodedButtonConfig::checkButtons() const {
  uint8_t virtualPin = getVirtualPin();
  for (uint8_t i = 0; i < mNumButtons; i++) {
    AceButton* button = mButtons[i];
    if (button == nullptr) continue;

    // For each button, call checkState() to allow it to figure out which
    // state it should move it.
    uint8_t buttonPin = button->getPin();
    uint8_t buttonState = (buttonPin == virtualPin)
        ? mPressedState : (mPressedState ^ 0x1);
    button->checkState(buttonState);
  }
}

uint8_t EncodedButtonConfig::getVirtualPin() const {
  uint8_t virtualPin = 0;
  for (uint8_t i = 0; i < mNumPins; i++) {
    uint8_t pin = mPins[i];
    int s = digitalRead(pin);
    virtualPin |= (s == mPressedState) << i;
  }
  return virtualPin;
}

}
