#line 2 "IEventHandlerTest.ino"

#include <AUnit.h>
#include <AceButton.h>
#include <ace_button/testing/TestableButtonConfig.h>
#include <ace_button/testing/EventTracker.h>
#include <ace_button/testing/HelperForButtonConfig.h>

using namespace aunit;
using namespace ace_button;
using namespace ace_button::testing;

// --------------------------------------------------------------------------

class EventHandlerClass: public IEventHandler {
  public:
    EventHandlerClass(EventTracker* eventTracker):
      eventTracker_(eventTracker) {}

    void handleEvent(AceButton* button, uint8_t eventType,
        uint8_t buttonState) override {
      eventTracker_->addEvent(button->getPin(), eventType, buttonState);
    }

  private:
    EventTracker* eventTracker_;
};

TestableButtonConfig testableConfig;
AceButton button(&testableConfig);
EventTracker eventTracker;
HelperForButtonConfig helper(&testableConfig, &button, &eventTracker);
EventHandlerClass eventHandler(&eventTracker);

const uint8_t PIN = 13;
const uint8_t BUTTON_ID = 1;

void setup() {
#if ! defined(EPOXY_DUINO)
  delay(1000); // wait to prevent garbage on SERIAL_PORT_MONITOR
#endif

  SERIAL_PORT_MONITOR.begin(115200);
  while (!SERIAL_PORT_MONITOR); // wait until ready - Leonardo/Micro only

  testableConfig.setIEventHandler(&eventHandler);
}

void loop() {
  TestRunner::run();
}

// --------------------------------------------------------------------------
// Test IEventHandler
// --------------------------------------------------------------------------

test(press_and_release_pullup) {
  const uint8_t DEFAULT_RELEASED_STATE = HIGH;
  uint8_t expected;

  // reset the button
  helper.init(PIN, DEFAULT_RELEASED_STATE, BUTTON_ID);

  // initial button state
  helper.releaseButton(0);
  assertEqual(0, eventTracker.getNumEvents());

  // must wait until the initial debouncing
  helper.releaseButton(50);
  assertEqual(0, eventTracker.getNumEvents());

  // button pressed, but must wait to debounce
  helper.pressButton(100);
  assertEqual(0, eventTracker.getNumEvents());

  // still in debouncing period, so no event yet
  helper.releaseButton(110);
  assertEqual(0, eventTracker.getNumEvents());

  // after more than 50 ms, we should get an event
  helper.pressButton(190);
  assertEqual(1, eventTracker.getNumEvents());
  expected = AceButton::kEventPressed;
  assertEqual(expected, eventTracker.getRecord(0).getEventType());
  assertEqual(LOW, eventTracker.getRecord(0).getButtonState());

  // release the button
  helper.releaseButton(1000);
  assertEqual(0, eventTracker.getNumEvents());

  // wait more than 50 ms
  helper.releaseButton(1060);
  assertEqual(1, eventTracker.getNumEvents());
  expected = AceButton::kEventReleased;
  assertEqual(expected, eventTracker.getRecord(0).getEventType());
  assertEqual(HIGH, eventTracker.getRecord(0).getButtonState());
}
