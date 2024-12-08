#ifndef _LED_DEVICE_H
#define _LED_DEVICE_H

#include <TaskManagerIO.h>

#include "settings.h"

namespace tstat {

class HeatingLedDevice {
 private:
  bool _isOn = false;
  const unsigned short _PIN = settings::HEATING_LED_PIN;
  const unsigned short _BRIGHTNESS_VALUE =
      settings::HEATING_LED_BRIGHTNESS_VALUE;

 public:
  void setup();
  void switchOn();
  void switchOff();
  void toggle();
  void startBlinking();
  void stopBlinking();
  taskid_t blinkTaskId = TASKMGR_INVALIDID;
};

// "Soft" singleton global object defined as extern and initialized here,
//  but also defined in led-device.cpp.
extern HeatingLedDevice heatingLedDevice;

class ErrorLedDevice {
 private:
  bool _isOn = false;
  const unsigned short _PIN = settings::ERROR_LED_PIN;
  const unsigned short _BRIGHTNESS_VALUE = settings::ERROR_LED_BRIGHTNESS_VALUE;

 public:
  void setup();
  void switchOn();
  void switchOff();
  void toggle();
  void startBlinking();
  void stopBlinking();
  taskid_t blinkTaskId = TASKMGR_INVALIDID;
};

// "Soft" singleton global object defined as extern and initialized here,
//  but also defined in led-device.cpp.
extern ErrorLedDevice errorLedDevice;

class DomainLedDevice {
 private:
  bool _isOn = false;
  const unsigned short _PIN = settings::DOMAIN_LED_PIN;
  const unsigned short _BRIGHTNESS_VALUE =
      settings::DOMAIN_LED_BRIGHTNESS_VALUE;

 public:
  void setup();
  void switchOn();
  void switchOff();
};

// "Soft" singleton global object defined as extern and initialized here,
//  but also defined in led-device.cpp.
extern DomainLedDevice domainLedDevice;

}  // namespace tstat
#endif
