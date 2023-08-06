#line 2 "EncodedButtonConfigTest.ino"

#include <AUnit.h>
#include <AceButton.h>
#include <ace_button/testing/TestableEncodedButtonConfig.h>
#include <ace_button/testing/EventTracker.h>
#include <ace_button/testing/HelperForEncodedButtonConfig.h>

using namespace aunit;
using namespace ace_button;
using namespace ace_button::testing;

// --------------------------------------------------------------------------

// Create an array of actual pins used.
static const uint8_t NUM_PINS = 4;
static const uint8_t PINS[] = {2, 3, 4, 5};

// Create 15 AceButton objects.
// Note: we could use an array of AceButton BUTTONS[15], and use a loop to
// initialize these arrays, but this is more explicit and easier to understand
// as an example code.
// AceButton b00 cannot be created because it is used to indicate "no button
// pressed".
static const uint8_t NUM_BUTTONS = 15;
static AceButton b01(1);
static AceButton b02(2);
static AceButton b03(3);
static AceButton b04(4);
static AceButton b05(5);
static AceButton b06(6);
static AceButton b07(7);
static AceButton b08(8);
static AceButton b09(9);
static AceButton b10(10);
static AceButton b11(11);
static AceButton b12(12);
static AceButton b13(13);
static AceButton b14(14);
static AceButton b15(15);
static AceButton* const BUTTONS[] = {
    &b01, &b02, &b03, &b04, &b05, &b06, &b07,
    &b08, &b09, &b10, &b11, &b12, &b13, &b14, &b15,
};

static TestableEncodedButtonConfig testableConfig(
    NUM_PINS, PINS, NUM_BUTTONS, BUTTONS);
static EventTracker eventTracker;
static HelperForEncodedButtonConfig helper(&testableConfig, &eventTracker);

// Store the arguments passed into the event handler into the EventTracker
// for assertion later.
void handleEvent(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  eventTracker.addEvent(button->getPin(), eventType, buttonState);
}

void setup() {
#if ! defined(EPOXY_DUINO)
  delay(1000); // wait to prevent garbage on SERIAL_PORT_MONITOR
#endif

  SERIAL_PORT_MONITOR.begin(115200);
  while (!SERIAL_PORT_MONITOR); // wait until ready - Leonardo/Micro only

  testableConfig.setEventHandler(handleEvent);
}

void loop() {
  TestRunner::run();
}

// --------------------------------------------------------------------------
// EncodedButtonConfig
// --------------------------------------------------------------------------

test(EncodedButtonConfig, press_and_release_pullup) {
  const unsigned long BASE_TIME = 65500; // rolls over in 36 milliseconds
  helper.init();

  // start the AceButton.check()
  helper.releaseButton(BASE_TIME);

  // initialization phase
  helper.releaseButton(BASE_TIME + 50);

  // Press button 1
  helper.pressButton(BASE_TIME + 100, 1);
  assertEqual(0, eventTracker.getNumEvents());

  // Press button 1 still debouncing
  helper.pressButton(BASE_TIME + 110, 1);
  assertEqual(0, eventTracker.getNumEvents());

  // After more than 20ms, button 1 press registers
  helper.pressButton(BASE_TIME + 130, 1);
  assertEqual(1, eventTracker.getNumEvents());
  {
    const EventRecord& record = eventTracker.getRecord(0);
    assertEqual(AceButton::kEventPressed, record.getEventType());
    assertEqual(1, record.getPin());
    assertEqual(LOW, record.getButtonState());
  }

  // release the button, still debouncing
  helper.releaseButton(BASE_TIME + 1000);
  assertEqual(0, eventTracker.getNumEvents());

  // release the button, after more 20ms, register a release
  helper.releaseButton(BASE_TIME + 1030);
  assertEqual(1, eventTracker.getNumEvents());
  {
    const EventRecord& record = eventTracker.getRecord(0);
    assertEqual(AceButton::kEventReleased, record.getEventType());
    assertEqual(1, record.getPin());
    assertEqual(HIGH, record.getButtonState());
  }
}

test(EncodedButtonConfig, click) {
  const unsigned long BASE_TIME = 65500; // rolls over in 36 milliseconds
  helper.init();
  testableConfig.setFeature(ButtonConfig::kFeatureClick);

  // Start the AceButton.check().
  helper.releaseButton(BASE_TIME);

  // Initialization phase.
  helper.releaseButton(BASE_TIME + 50);

  // Press button 2.
  helper.pressButton(BASE_TIME + 100, 2);
  assertEqual(0, eventTracker.getNumEvents());

  // Press button 2 still debouncing.
  helper.pressButton(BASE_TIME + 110, 2);
  assertEqual(0, eventTracker.getNumEvents());

  // After more than 20ms, button 2 press registers.
  helper.pressButton(BASE_TIME + 130, 2);
  assertEqual(1, eventTracker.getNumEvents());
  {
    const EventRecord& record = eventTracker.getRecord(0);
    assertEqual(AceButton::kEventPressed, record.getEventType());
    assertEqual(2, record.getPin());
    assertEqual(LOW, record.getButtonState());
  }

  // Continue pressing button for 100ms.
  helper.pressButton(BASE_TIME + 230, 2);
  assertEqual(0, eventTracker.getNumEvents());

  // Release before 200 ms (from start of pressed event); waiting for
  // debouncing.
  helper.releaseButton(BASE_TIME + 300);
  assertEqual(0, eventTracker.getNumEvents());

  // Debouncing finished after 20 ms.
  helper.releaseButton(BASE_TIME + 320);
  assertEqual(2, eventTracker.getNumEvents());
  {
    const EventRecord& record = eventTracker.getRecord(0);
    assertEqual(AceButton::kEventClicked, record.getEventType());
    assertEqual(2, record.getPin());
    assertEqual(HIGH, record.getButtonState());
  }
  {
    const EventRecord& record = eventTracker.getRecord(1);
    assertEqual(AceButton::kEventReleased, record.getEventType());
    assertEqual(2, record.getPin());
    assertEqual(HIGH, record.getButtonState());
  }
}

