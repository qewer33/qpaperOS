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

#ifndef ACE_BUTTON_EVENT_TRACKER_H
#define ACE_BUTTON_EVENT_TRACKER_H

#include <Arduino.h> // LOW

namespace ace_button {
namespace testing {

/**
 * A record of an AceButton event, for testing purposes.
 */
class EventRecord {
  public:
    EventRecord():
        mPin(0),
        mEventType(0),
        mButtonState(LOW) {}

    EventRecord(uint8_t pin, uint8_t eventType, uint8_t buttonState):
        mPin(pin),
        mEventType(eventType),
        mButtonState(buttonState) {}

    uint8_t getPin() const {
      return mPin;
    }

    uint8_t getEventType() const {
      return mEventType;
    }

    uint8_t getButtonState() const {
      return mButtonState;
    }

    void printTo(Print& printer) const;

    // Accept the default copy-constructor and assignment operator.
    EventRecord(const EventRecord&) = default;
    EventRecord& operator=(const EventRecord&) = default;

  private:
    uint8_t mPin;
    uint8_t mEventType;
    uint8_t mButtonState;
};

/**
 * Class that can receive and remember multiple calls to the eventHandler from
 * AceButton.
 */
class EventTracker {
  public:

    EventTracker():
        mNumEvents(0) {}
      
    /** Add event to a buffer of records, stopping when the buffer fills up. */
    void addEvent(uint8_t pin, uint8_t eventType, uint8_t buttonState) {
      if (mNumEvents < kMaxEvents) {
        mRecords[mNumEvents] = EventRecord(pin, eventType, buttonState);
        mNumEvents++;
      }
    }

    void clear() { mNumEvents = 0; }

    int getNumEvents() { return mNumEvents; }

    EventRecord& getRecord(int i) { return mRecords[i]; }

    void printTo(Print& printer) const;

  private:
    // Disable copy-constructor and assignment operator
    EventTracker(const EventTracker&) = delete;
    EventTracker& operator=(const EventTracker&) = delete;

    // Don't expect more than about 3. Set to 5 just in case.
    static const int kMaxEvents = 5;

    EventRecord mRecords[kMaxEvents];
    int mNumEvents;
};

}
}
#endif
