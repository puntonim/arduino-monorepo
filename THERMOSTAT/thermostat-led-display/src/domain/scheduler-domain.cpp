/* SchedulerDomain is just a dumb cointainer class that includes:
 *  - the timer
 *  - the target temperature.
 * It does NOT tick the timer and doesn't check the tearget temperature against
 *  the sensor temperature.
 */
#include "domain/scheduler-domain.h"

#include "utils/pubsub-utils.h"
#include "utils/time-utils.h"

namespace tstat {

// "Soft" singleton global object defined here,
//  but defined as extern and initialized in scheduler-domain.h.
SchedulerDomain schedulerDomain;

void SchedulerDomain::setup() {
  // SUBSCRIPTIONS.
  pubsub_utils::pubSub.subscribe(
      [this](pubsub_utils::TimerRotaryChangeEvent* pEvent) {
#if IS_DEBUG == true
        Serial.println((String) "SchedulerDomain - received event: " +
                       pEvent->topic);
#endif
        this->_onTimerRotaryChange(pEvent);
      });

  pubsub_utils::pubSub.subscribe(
      [this](pubsub_utils::TargetTRotaryChangeEvent* pEvent) {
#if IS_DEBUG == true
        Serial.println((String) "SchedulerDomain - received event: " +
                       pEvent->topic);
#endif
        this->_onTargetTRotaryChange(pEvent);
      });
}

bool SchedulerDomain::isScheduled() { return !timer.isOver(); }

/**
 * Handle the timer rotary encoder rotation.
 *
 * - if the display was OFF: noop (as we just have to switch on
 *    the display and this is done in display-device.cpp)
 * - if the display was ON and nothing was scheduled (SPENTO):
 *    - if the rotation was clockwise then start a new timer with the initial
 *       timer setting
 *    - if the rotation was clockwise then noop
 * - if the display was ON and there was an ongoing schedule: just add or
 *    subtract time
 */
void SchedulerDomain::_onTimerRotaryChange(
    pubsub_utils::TimerRotaryChangeEvent* pEvent) {
  // If the rotary encoder was rotated when the display was OFF, then noop (as
  //  as we just have to switch on the display and NOT to increment the timer).
  if (pEvent->isDisplayOn) {
    if (timer.isOver() && pEvent->value > 0) {
      // If time is over and the timer rotary encoder was rotated clockwise,
      //  then start a new timer with the initial time.
      timer.start(settings::INITIAL_TIMER.h, settings::INITIAL_TIMER.m,
                  settings::INITIAL_TIMER.s);
      // And publish the new schedule event.
      pubsub_utils::pubSub.publish(new pubsub_utils::NewScheduleEvent());
    } else {
      // If time is not over, then add/remove the time.
      time_utils::Time time;
      if (pEvent->value > 0) {
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
      // And publish the edit time event.
      pubsub_utils::pubSub.publish(
          new pubsub_utils::SchedulerEditTimeEvent(time));
    }
  }
}

/**
 * Handle the target T rotary encoder rotation.
 *
 * - if the display was OFF: noop (as we just have to switch on
 *    the display and this is done in display-device.cpp)
 * - if the display was ON and nothing was scheduled (SPENTO): noop
 * - if the display was ON and there was an ongoing schedule: just increase or
 *    decrease the target T
 */
void SchedulerDomain::_onTargetTRotaryChange(
    pubsub_utils::TargetTRotaryChangeEvent* pEvent) {
  // If the rotary encoder was rotated when the display was OFF, then noop (as
  //  we have to switch on the display and NOT to increment the target T).
  if (pEvent->isDisplayOn) {
    if (isScheduled()) {
      // If there is an ongoing schedule, then increase/decrease the target T.
      targetTemperature = pEvent->value;
      pubsub_utils::pubSub.publish(
          new pubsub_utils::SchedulerEditTargetTEvent(targetTemperature));
    }
  }
}

void SchedulerDomain::reset() {
  timer.reset();
  targetTemperature = settings::INITIAL_TARGET_T;
}
}  // namespace tstat