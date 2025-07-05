#ifndef _TIMER_DOMAIN_H
#define _TIMER_DOMAIN_H

#include <TaskManagerIO.h>

namespace bigpjtemplate {

class TimerDomain {
 private:
 public:
  void setup();
  void onTimerFinished();
  taskid_t timerTaskId = TASKMGR_INVALIDID;
};

// "Soft" *singleton* global object defined as extern and initialized here,
//  but also defined in timer-domain.cpp. `extern` just means that the var is
//  defined elsewhere. And we need it here because we want it initialized on
//  on boot.
extern TimerDomain timerDomain;

}  // namespace bigpjtemplate

#endif