
#include "led-device.h"

namespace oopexample {

// "Soft" *singleton* global object defined here,
//  but defined as extern and initialized in led-device.h.
ErrorLedDevice errorLedDevice;

void ErrorLedDevice::setup() { pinMode(_PIN, OUTPUT); }

void ErrorLedDevice::switchOn() {
  if (!_isOn) {
    analogWrite(_PIN, _BRIGHTNESS_VALUE);
    _isOn = true;
  }
}

void ErrorLedDevice::switchOff() {
  if (_isOn) {
    analogWrite(_PIN, 0);
    _isOn = false;
  }
}

void ErrorLedDevice::toggle() { _isOn ? switchOff() : switchOn(); }

void ErrorLedDevice::startBlinking() {
  // Start the blinking task only if it's not already running.
  if (blinkTaskId == TASKMGR_INVALIDID) {
    Serial.println((String) "ErrorLedDevice - starting a new blinking task");
    blinkTaskId =
        taskManager.scheduleFixedRate(100, [] { errorLedDevice.toggle(); });
  }
}

void ErrorLedDevice::stopBlinking() {
  Serial.println((String) "ErrorLedDevice - stopping the blinking task");
  taskManager.cancelTask(blinkTaskId);
  blinkTaskId = TASKMGR_INVALIDID;
  // Make sure the led is off as the task might have left it on.
  errorLedDevice.switchOff();
}

}  // namespace oopexample
