#ifndef _SCHEDULER_DOMAIN_H
#define _SCHEDULER_DOMAIN_H

#include "settings.h"
#include "utils/pubsub-utils.h"
#include "utils/time-utils.h"

namespace tstat {

class SchedulerDomain {
 private:
  bool _isScheduled = false;
  void _onTimerButtonPress(pubsub_utils::TimerButtonPressEvent* pEvent);
  void _addTime(unsigned short int h, unsigned short int m);
  void _setTargetTemperature(unsigned short int t);

 public:
  void setup();
  time_utils::Timer timer;
  unsigned short int targetTemperature = settings::DEFAULT_TARGET_T;
  bool isScheduled();
};

// "Soft" singleton global object defined as extern and initialized here,
//  but also defined in scheduler-domain.cpp.
extern SchedulerDomain schedulerDomain;

}  // namespace tstat
#endif