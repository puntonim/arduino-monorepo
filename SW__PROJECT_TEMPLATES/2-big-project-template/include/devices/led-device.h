#ifndef _LED_DEVICE_H
#define _LED_DEVICE_H

#include <Arduino.h>  // Required by `LED_BUILTIN`.
#include <TaskManagerIO.h>

#include "Arduino_LED_Matrix.h"
#include "settings.h"

namespace myproject {

//
//********** CLASS ErrorLedDevice **********************************************
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

// "Soft" *singleton* global object defined as extern and initialized here,
//  but also defined in led-device.cpp. `extern` just means that the var is
//  defined elsewhere. And we need it here because we want it initialized on
//  on boot.
extern ErrorLedDevice errorLedDevice;

//
//********** CLASS DomainLedDevice *********************************************
class DomainLedDevice {
 private:
  bool _isOn = false;
  unsigned long _lastStatusChangeTs = 0;
  const unsigned short _PIN = settings::DOMAIN_LED_PIN;
  const unsigned short _BRIGHTNESS_VALUE =
      settings::DOMAIN_LED_BRIGHTNESS_VALUE;

 public:
  void setup();
  void switchOn(const bool doUseDelay = false);
  void switchOff(const bool doUseDelay = false);
};

// "Soft" singleton global object defined as extern and initialized here,
//  but also defined in led-device.cpp.
extern DomainLedDevice domainLedDevice;

//
//********** CLASS LedMatrixDevice *********************************************
class LedMatrixDevice {
 private:
  uint8_t _frame[8][12] = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                           {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                           {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                           {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                           {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                           {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                           {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                           {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
  ArduinoLEDMatrix _matrix;

 public:
  void setup();
  void switchOn();
  void switchOff();
};

// "Soft" singleton global object defined as extern and initialized here,
//  but also defined in led-device.cpp.
extern LedMatrixDevice ledMatrixDevice;

}  // namespace myproject

#endif