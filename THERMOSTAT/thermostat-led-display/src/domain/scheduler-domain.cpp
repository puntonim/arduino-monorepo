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
      [this](pubsub_utils::TimerButtonPressEvent* pEvent) {
#if IS_DEBUG == true
        Serial.println((String) "SchedulerDomain - received event: " +
                       pEvent->topic);
#endif
        this->_onTimerButtonPress(pEvent);
      });
}

bool SchedulerDomain::isScheduled() { return !timer.isOver(); }

void SchedulerDomain::_onTimerButtonPress(
    pubsub_utils::TimerButtonPressEvent* pEvent) {
  // If the button was pressed when the display was OFF, then noop (as we just
  //  have to switch on the display and NOT to increment the timer).
  if (pEvent->isDisplayOn) {
    _addTime(0, 15);
    // If nothing is scheduled, then we need to start a new schedule.
    if (!_isScheduled)
      pubsub_utils::pubSub.publish(new pubsub_utils::NewScheduleEvent());
  }
}

void SchedulerDomain::_addTime(unsigned short int h, unsigned short int m) {
  time_utils::Time time = timer.addTime(0, 0, 2);
  pubsub_utils::pubSub.publish(new pubsub_utils::SchedulerEditTimeEvent(time));
}

}  // namespace tstat