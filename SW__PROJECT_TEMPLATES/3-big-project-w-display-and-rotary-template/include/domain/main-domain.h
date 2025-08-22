#ifndef _MAIN_DOMAIN_H
#define _MAIN_DOMAIN_H

#include <TaskManagerIO.h>

#include "utils/pubsub-utils.h"
#include "utils/time-utils.h"

namespace myproject {

class MainDomain {
 private:
  void _onTimerRotaryRotation(pubsub_utils::TimerRotaryRotationEvent* pEvent);
  void _onTimerEnd();
  void _handleTestError();
  void _onAnyRotaryHoldEvent();

 public:
  time_utils::Timer timer;
  void setup();
  void run();
  struct time_utils::Time tick();
  taskid_t runTaskId = TASKMGR_INVALIDID;
};

// "Soft" *singleton* global object defined as extern and initialized here,
//  but also defined in main-domain.cpp. `extern` just means that the var is
//  defined elsewhere. And we need it here because we want it initialized on
//  on boot.
extern MainDomain mainDomain;

}  // namespace myproject

#endif