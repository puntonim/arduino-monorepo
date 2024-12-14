#ifndef _SCHEDULER_DOMAIN_H
#define _SCHEDULER_DOMAIN_H

#include "settings.h"
#include "utils/pubsub-utils.h"
#include "utils/time-utils.h"

namespace tstat {

class SchedulerDomain {
 private:
  void _onTimerRotaryChange(pubsub_utils::TimerRotaryChangeEvent* pEvent);
  void _onTargetTRotaryChange(pubsub_utils::TargetTRotaryChangeEvent* pEvent);

 public:
  void setup();
  void reset();
  time_utils::Timer timer;
  u_int8_t targetTemperature = settings::INITIAL_TARGET_T;
  bool isScheduled();
};

// "Soft" singleton global object defined as extern and initialized here,
//  but also defined in scheduler-domain.cpp.
extern SchedulerDomain schedulerDomain;

}  // namespace tstat
#endif