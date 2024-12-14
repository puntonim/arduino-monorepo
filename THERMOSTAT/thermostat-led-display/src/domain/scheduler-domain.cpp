/* SchedulerDomain is just a dumb cointainer class that includes a timer
 *  and the target temperature.
 * It doesn't tick the timer and doesn't check the tearget temperature against
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
}

bool SchedulerDomain::isScheduled() { return !timer.isOver(); }

void SchedulerDomain::_onTimerRotaryChange(
    pubsub_utils::TimerRotaryChangeEvent* pEvent) {
  // If the button was pressed when the display was OFF, then noop (as we just
  //  have to switch on the display and NOT to increment the timer).
  if (pEvent->isDisplayOn) {
    if (timer.isOver() && pEvent->value > 0) {
      // If time is over and the timer rotary was rotated clockwise,
      //  then start a new timer with the default time.
      timer.start(settings::DEFAULT_TIMER.h, settings::DEFAULT_TIMER.m,
                  settings::DEFAULT_TIMER.s);
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

}  // namespace tstat