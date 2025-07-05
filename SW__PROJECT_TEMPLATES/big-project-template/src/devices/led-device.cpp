#include "devices/led-device.h"

#include "settings.h"
#include "utils/pubsub-utils.h"

namespace bigpjtemplate {

// "Soft" *singleton* global object defined here,
//  but defined as extern and initialized in led-device.h.
ErrorLedDevice errorLedDevice;

void ErrorLedDevice::setup() {
  pinMode(_PIN, OUTPUT);

  // SUBSCRIPTION TimerFinishEvent ---------------------------------------------
  pubsub_utils::pubSub.subscribe(
      [this](pubsub_utils::TimerFinishEvent* pEvent) {
#if IS_DEBUG == true
        Serial.println((String) "ErrorLedDevice - received event: " +
                       pEvent->topic + " tsMillis=" + pEvent->tsMillis);
#endif
        this->startBlinking();
      });
}

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
#if IS_DEBUG == true
    Serial.println((String) "ErrorLedDevice - starting a new blinking task");
#endif
    blinkTaskId =
        taskManager.scheduleFixedRate(100, [] { errorLedDevice.toggle(); });
  }
}

void ErrorLedDevice::stopBlinking() {
#if IS_DEBUG == true
  Serial.println((String) "ErrorLedDevice - stopping the blinking task");
#endif
  taskManager.cancelTask(blinkTaskId);
  blinkTaskId = TASKMGR_INVALIDID;
  // Make sure the led is off as the task might have left it on.
  errorLedDevice.switchOff();
}

}  // namespace bigpjtemplate
