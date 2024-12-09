#ifndef _BUTTON_DEVICE_H
#define _BUTTON_DEVICE_H

#include "settings.h"

namespace tstat {

class ButtonDevices {
 private:
 public:
  void setup();
};

// "Soft" singleton global object defined as extern and initialized here,
//  but also defined in button-device.cpp.
extern ButtonDevices buttonDevices;

}  // namespace tstat
#endif
