#ifndef _LED_DEVICE_H
#define _LED_DEVICE_H

#include <Arduino.h>  // Required by `LED_BUILTIN`.
#include <TaskManagerIO.h>

namespace oopexample {

class ErrorLedDevice {
 private:
  bool _isOn = false;
  const unsigned short _PIN = LED_BUILTIN;
  const unsigned short _BRIGHTNESS_VALUE = 100;

 public:
  void setup();
  void switchOn();
  void switchOff();
  void toggle();
  void startBlinking();
  void stopBlinking();
  taskid_t blinkTaskId = TASKMGR_INVALIDID;
};

// "Soft" *singleton* global object defined as extern and initialized here,
//  but also defined in led-device.cpp. `extern` just means that the var is
//  defined elsewhere. And we need it here because we want it initialized on
//  on boot.
extern ErrorLedDevice errorLedDevice;

}  // namespace oopexample

#endif