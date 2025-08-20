#ifndef _MAIN_DOMAIN_H
#define _MAIN_DOMAIN_H

#include <TaskManagerIO.h>

namespace bigpjtemplate {

class MainDomain {
 private:
 public:
  void setup();
  void run();
  taskid_t runTaskId = TASKMGR_INVALIDID;
};

// "Soft" *singleton* global object defined as extern and initialized here,
//  but also defined in main-domain.cpp. `extern` just means that the var is
//  defined elsewhere. And we need it here because we want it initialized on
//  on boot.
extern MainDomain mainDomain;

}  // namespace bigpjtemplate

#endif