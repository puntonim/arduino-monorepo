#include "domain/timer-domain.h"

#include "settings.h"
#include "utils/pubsub-utils.h"

namespace bigpjtemplate {

// "Soft" *singleton* global object defined here,
//  but defined as extern and initialized in timer-domain.h.
TimerDomain timerDomain;

void TimerDomain::setup() {
#if IS_DEBUG == true
  Serial.println((String) "Starting a timer by scheduling a task once in " +
                 settings::TIMER_DURATION_SEC + " secs");
#endif
  timerTaskId = taskManager.schedule(onceSeconds(settings::TIMER_DURATION_SEC),
                                     [] { timerDomain.onTimerFinished(); });
}

void TimerDomain::onTimerFinished() {
#if IS_DEBUG == true
  Serial.println((String) "The timer is over, publishing event...");
#endif
  pubsub_utils::pubSub.publish(new pubsub_utils::TimerFinishEvent(millis()));
}

}  // namespace bigpjtemplate
