#include "domain/main-domain.h"

#include "devices/led-device.h"
#include "settings.h"
#include "utils/error-utils.h"

namespace myproject {

// "Soft" *singleton* global object defined here,
//  but defined as extern and initialized in main-domain.h.
MainDomain mainDomain;

void MainDomain::setup() {
  // SUBSCRIPTION TimerRotaryRotationEvent -------------------------------------
  pubsub_utils::pubSub.subscribe(
      [this](pubsub_utils::TimerRotaryRotationEvent event) {
#if IS_DEBUG == true
        Serial.println((String) "MainDomain - received event: " + event.topic);
#endif
        this->_onTimerRotaryRotation(event);
      });

  // SUBSCRIPTION AnyRotaryHoldEvent -------------------------------------------
  pubsub_utils::pubSub.subscribe(
      [this](pubsub_utils::AnyRotaryHoldEvent event) {
#if IS_DEBUG == true
        Serial.println((String) "MainDomain - received event: " + event.topic);
#endif
        this->_onAnyRotaryHoldEvent();
      });

  // SUBSCRIPTION TimerEndEvent --------------------------------------------
  pubsub_utils::pubSub.subscribe([this](pubsub_utils::TimerEndEvent event) {
#if IS_DEBUG == true
    Serial.println((String) "MainDomain - received event: " + event.topic);
#endif
    this->_onTimerEnd();
  });

  // ---------------------------------------------------------------------------
#if IS_DEBUG == true
  Serial.println((String) "MainDomain - scheduling a run every " +
                 settings::MAIN_DOMAIN_RUN_PERIOD + " sec");
#endif

  runTaskId =
      taskManager.schedule(repeatSeconds(settings::MAIN_DOMAIN_RUN_PERIOD),
                           [] { mainDomain.run(); });
}

void MainDomain::run() {
#if IS_DEBUG == true
  Serial.println("MainDomain - run");
#endif

  domainLedDevice.switchOn(true);

  tick();
  _handleTestError();

  domainLedDevice.switchOff(true);
}

/**
 * Add a test error after ~20 sec after boot and remove it 5 secs later.
 * The error should be printed to the Serial Monitor, sent to io-be at
 *  /iot/remote-logs/ and the errorLed should blink.
 */
bool wasErrorAdded = false;
void MainDomain::_handleTestError() {
  auto nowTs = millis();
  // After 20 secs from boot, add a test error for 10 secs.
  if (nowTs > 20000 && !wasErrorAdded) {
    error_utils::errorMgr.addError("TEST_ERROR", "Just a test error",
                                   "Test err");
    wasErrorAdded = true;
    taskManager.schedule(onceSeconds(10), [] {
      error_utils::errorMgr.removeError("TEST_ERROR");
      // errorDomain.removeAllErrors();
    });
  }
}

/**
 * A single tick on the countdown timer.
 */
struct time_utils::Time MainDomain::tick() {
  // If the time is already over, then no need to publish the event.
  if (timer.isOver()) return timer.getTime();
  struct time_utils::Time time = timer.tick();
  if (timer.isOver())
    pubsub_utils::pubSub.publish(pubsub_utils::TimerEndEvent());
  return time;
}

/**
 * When the countdown timer reaches 0, switch ON the LED matrix for 20 secs.
 */
void MainDomain::_onTimerEnd() {
  ledMatrixDevice.switchOn();
  taskManager.schedule(onceSeconds(20), [] { ledMatrixDevice.switchOff(); });
}

/**
 * When the rotary encoder is held, reset the countdown timer.
 */
void MainDomain::_onAnyRotaryHoldEvent() {
  bool wasOver = timer.isOver();
  timer.reset();
  if (!wasOver) pubsub_utils::pubSub.publish(pubsub_utils::TimerEndEvent());
}

/**
 * Handle the timer rotary encoder rotation.
 *
 * - if the display was OFF: noop (as we just have to switch on
 *    the display and this is done in display-device.cpp)
 * - if the display was ON and nothing was scheduled (SPENTO):
 *    - if the rotation was clockwise then start a new countdown timer with the
 *       initial timer setting
 *    - if the rotation was clockwise then noop
 * - if the display was ON and there was an ongoing schedule: just add or
 *    subtract time
 */
void MainDomain::_onTimerRotaryRotation(
    pubsub_utils::TimerRotaryRotationEvent event) {
  // If the rotary encoder was rotated when the display was OFF, then noop (as
  //  as we just have to switch on the display and NOT to increment the
  //  countdown timer).
  if (!event.isDisplayOn) return;
  if (timer.isOver()) {
    if (event.value > 0) {
      // If time is over and the timer rotary encoder was rotated clockwise,
      //  then start a new countdown timer with the initial time.
      timer.start(settings::INITIAL_TIMER.h, settings::INITIAL_TIMER.m,
                  settings::INITIAL_TIMER.s);
      // And publish the new schedule event.
      pubsub_utils::pubSub.publish(pubsub_utils::TimerStartEvent());
    }
  } else {
    // If time is not over, then add/remove the time.
    time_utils::Time time;
    if (event.value > 0) {
      // Clockwise rotation: add time.
      time = timer.add(settings::DELTA_TIME_ON_ROTARY_ROTATION.h,
                       settings::DELTA_TIME_ON_ROTARY_ROTATION.m,
                       settings::DELTA_TIME_ON_ROTARY_ROTATION.s);
    } else {
      // Counterclockwise rotation: subtract time.
      time = timer.subtract(settings::DELTA_TIME_ON_ROTARY_ROTATION.h,
                            settings::DELTA_TIME_ON_ROTARY_ROTATION.m,
                            settings::DELTA_TIME_ON_ROTARY_ROTATION.s);
    }
    timer.tick();
    if (!timer.isOver()) {
      pubsub_utils::pubSub.publish(pubsub_utils::TimerUpdateEvent(time));
    } else {
      pubsub_utils::pubSub.publish(pubsub_utils::TimerEndEvent());
    }
  }
}

}  // namespace myproject
