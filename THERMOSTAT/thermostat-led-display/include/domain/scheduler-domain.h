#ifndef _SCHEDULER_DOMAIN_H
#define _SCHEDULER_DOMAIN_H

#include "settings.h"
#include "utils/time-utils.h"

namespace tstat {

class SchedulerDomain {
 private:
  bool _isScheduled = false;

 public:
  void setup();
  time_utils::Timer timer;
  unsigned short int targetTemperature = settings::DEFAULT_TARGET_T;
  void addTime(unsigned short int h, unsigned short int m);
  void setTargetTemperature(unsigned short int t);
  bool isScheduled();
};

// "Soft" singleton global object defined as extern and initialized here,
//  but also defined in scheduler-domain.cpp.
extern SchedulerDomain schedulerDomain;

}  // namespace tstat
#endif