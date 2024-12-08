#ifndef _DOMAIN_H
#define _DOMAIN_H

#include <TaskManagerIO.h>

namespace tstat {

class Domain {
 private:
  bool _isHeatingOn = false;
  bool _checkForTemperature();
  bool _checkForTimer();
  void _switchHeatingOn();
  void _switchHeatingOff();

 public:
  void setup();
  void run();
  taskid_t runTaskId = TASKMGR_INVALIDID;
};

// "Soft" singleton global object defined as extern and initialized here,
//  but also defined in domain.cpp.
extern Domain domain;

}  // namespace tstat
#endif
