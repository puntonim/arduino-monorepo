#include "devices/led-device.h"

#include "settings.h"
#include "utils/pubsub-utils.h"
#include "utils/task-manager-utils.h"

namespace tstat {

//********** CLASS HeatingLedDevice ********************************************

// "Soft" singleton global object defined here,
//  but defined as extern and initialized in led-device.h.
HeatingLedDevice heatingLedDevice;

void HeatingLedDevice::setup() {
  pinMode(_PIN, OUTPUT);

  pubsub_utils::pubSub.subscribe(
      [this](pubsub_utils::HeatingStatusChangeEvent* pEvent) {
#if IS_DEBUG == true
        Serial.println(
            (String) "HeatingLedDevice - received event: " + pEvent->topic +
            " isOn=" + (pEvent->isOn ? "ON" : "OFF"));
#endif
        pEvent->isOn ? this->startBlinking() : this->stopBlinking();
      });
}

void HeatingLedDevice::switchOn() {
  analogWrite(_PIN, _BRIGHTNESS_VALUE);
  _isOn = true;
}

void HeatingLedDevice::switchOff() {
  analogWrite(_PIN, 0);
  _isOn = false;
}

void HeatingLedDevice::toggle() {
  if (_isOn)
    switchOff();
  else
    switchOn();
}

void HeatingLedDevice::startBlinking() {
  // Start the blinking task only if it's not already running.
  if (blinkTaskId == TASKMGR_INVALIDID) {
#if IS_DEBUG == true
    Serial.println((String) "HeatingLedDevice - starting a new blinking task");
#endif
    blinkTaskId =
        taskManager.scheduleFixedRate(settings::HEATING_LED_BLINKING_PERIOD,
                                      [] { heatingLedDevice.toggle(); });
  }
}

void HeatingLedDevice::stopBlinking() {
#if IS_DEBUG == true
  Serial.println((String) "HeatingLedDevice - stopping the blinking task");
#endif
  task_manager_utils::cancelTask(blinkTaskId);
  // Make sure the LED is off as the task might have left it on.
  switchOff();
}

//********** CLASS ErrorLedDevice **********************************************

// "Soft" singleton global object defined here,
//  but defined as extern and initialized in led-device.h.
ErrorLedDevice errorLedDevice;

// Note: at the moment the sensor SHT85 is only read by the display, so its
// error status is updated
//  only when the display is ON. This means that if the errorLed has been
//  triggered by an error on
//   the SHT85 sensor, then the errorLed status can be delayed until the display
//   is ON again.

void ErrorLedDevice::setup() {
  pinMode(_PIN, OUTPUT);

  pubsub_utils::pubSub.subscribe(
      [this](pubsub_utils::ErrorStatusChangeEvent* pEvent) {
#if IS_DEBUG == true
        Serial.println(
            (String) "ErrorLedDevice - received event: " + pEvent->topic +
            " isError=" + (pEvent->isError ? "ON" : "OFF"));
#endif
        if (pEvent->isError)
          this->startBlinking();
        else
          this->stopBlinking();
      });
}

void ErrorLedDevice::switchOn() {
  analogWrite(_PIN, _BRIGHTNESS_VALUE);
  _isOn = true;
}

void ErrorLedDevice::switchOff() {
  analogWrite(_PIN, 0);
  _isOn = false;
}

void ErrorLedDevice::toggle() { _isOn ? switchOff() : switchOn(); }

void ErrorLedDevice::startBlinking() {
  // Start the blinking task only if it's not already running.
  if (blinkTaskId == TASKMGR_INVALIDID) {
#if IS_DEBUG == true
    Serial.println((String) "ErrorLedDevice - starting a new blinking task");
#endif
    blinkTaskId = taskManager.scheduleFixedRate(
        settings::ERROR_LED_BLINKING_PERIOD, [] { errorLedDevice.toggle(); });
  }
}

void ErrorLedDevice::stopBlinking() {
#if IS_DEBUG == true
  Serial.println((String) "ErrorLedDevice - stopping the blinking task");
#endif
  task_manager_utils::cancelTask(blinkTaskId);
  // Make sure the led is off as the task might have left it on.
  switchOff();
}

//********** CLASS DomainLedDevice *********************************************

// "Soft" singleton global object defined here,
//  but defined as extern and initialized in led-device.h.
DomainLedDevice domainLedDevice;

void DomainLedDevice::setup() { pinMode(_PIN, OUTPUT); }

void DomainLedDevice::switchOn() {
  if (!_isOn) {
    analogWrite(_PIN, _BRIGHTNESS_VALUE);
    _isOn = true;
  }
}

void DomainLedDevice::switchOff() {
  if (_isOn) {
    analogWrite(_PIN, 0);
    _isOn = false;
  }
}

}  // namespace tstat
