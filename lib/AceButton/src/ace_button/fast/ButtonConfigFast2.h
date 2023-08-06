/*
MIT License

Copyright (c) 2021 Brian T. Park

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

#ifndef ACE_BUTTON_BUTTON_CONFIG_FAST2_H
#define ACE_BUTTON_BUTTON_CONFIG_FAST2_H

#include "../ButtonConfig.h"

namespace ace_button {

/**
 * An implementation of ButtonConfig that uses digitalReadFast() instead of
 * digitalRead() to support 2 buttons. The primary motivation for using
 * digitalReadFast() in this library is the reduction of flash memory, not the
 * improvement in performance.
 *
 * @tparam T_PIN0 physical pin used by button 0
 * @tparam T_PIN1 physical pin used by button 1
 */
template <uint8_t T_PIN0, uint8_t T_PIN1>
class ButtonConfigFast2 : public ButtonConfig {
  public:
    int readButton(uint8_t pin) override {
      // Using nested if-else statements instead of switch saves 2 bytes of
      // flash on an AVR. Not worth it.
      switch (pin) {
        case 0:
          return digitalReadFast(T_PIN0);
          break;
        case 1:
          return digitalReadFast(T_PIN1);
          break;
        default:
          return 0;
          break;
      }
    }
};

}
#endif
