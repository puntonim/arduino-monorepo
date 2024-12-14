#ifndef _ROTARY_ENCODER_DEVICE_H
#define _ROTARY_ENCODER_DEVICE_H

#include "settings.h"

namespace tstat {

class RotaryEncoderDevices {
 private:
 public:
  void setup();
};

// "Soft" singleton global object defined as extern and initialized here,
//  but also defined in rotary-encoder-device.cpp.
extern RotaryEncoderDevices rotaryEncoderDevices;

}  // namespace tstat
#endif
