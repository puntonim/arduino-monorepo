#include "devices/led-device.h"

#include "settings.h"
#include "utils/pubsub-utils.h"

namespace myproject {

//
//********** CLASS ErrorLedDevice **********************************************

// "Soft" *singleton* global object defined here,
//  but defined as extern and initialized in led-device.h.
ErrorLedDevice errorLedDevice;

void ErrorLedDevice::setup() {
  pinMode(_PIN, OUTPUT);

  // SUBSCRIPTION ErrorStatusUpdateEvent -------------------------------------
  pubsub_utils::pubSub.subscribe(
      [this](pubsub_utils::ErrorStatusUpdateEvent event) {
#if IS_DEBUG == true
        Serial.println((String) "ErrorLedDevice - received event: " +
                       event.topic + " isError=" + (event.isError ? "T" : "F"));
#endif
        event.isError ? this->startBlinking() : this->stopBlinking();
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

//
//********** CLASS DomainLedDevice *********************************************

// "Soft" singleton global object defined here,
//  but defined as extern and initialized in led-device.h.
DomainLedDevice domainLedDevice;

void DomainLedDevice::setup() { pinMode(_PIN, OUTPUT); }

/**
 * Switch ON the led.
 *
 * Args:
 *  const bool doUseDelay: when switching ON/OFF the led too fast, then the
 *   led does NOT even blink. To avoid this, use doUseDelay=true so that the
 *   led waits 200msec if the prev state change was too soon.
 */
void DomainLedDevice::switchOn(const bool doUseDelay /* = false */) {
  unsigned long nowTs;
  if (doUseDelay) {
    nowTs = millis();
    if ((!_isOn) && (_lastStatusChangeTs != 0) &&
        (nowTs - _lastStatusChangeTs < 200)) {
      taskManager.yieldForMicros(200000);  // microsec.
    }
  }
  if (!_isOn) {
    analogWrite(_PIN, _BRIGHTNESS_VALUE);
    _isOn = true;
    if (doUseDelay) _lastStatusChangeTs = nowTs;
  }
}

/**
 * Switch OFF the led.
 *
 * Args:
 *  const bool doUseDelay: when switching ON/OFF the led too fast, then the
 *   led does NOT even blink. To avoid this, use doUseDelay=true so that the
 *   led waits 200msec if the prev state change was too soon.
 */
void DomainLedDevice::switchOff(const bool doUseDelay /* = false */) {
  unsigned long nowTs;
  if (doUseDelay) {
    nowTs = millis();
    if ((_isOn) && (_lastStatusChangeTs != 0) &&
        (nowTs - _lastStatusChangeTs < 200)) {
      taskManager.yieldForMicros(200000);  // microsec.
    }
  }
  if (_isOn) {
    analogWrite(_PIN, 0);
    _isOn = false;
    if (doUseDelay) _lastStatusChangeTs = nowTs;
  }
}

//
//********** CLASS LedMatrixDevice *********************************************

// "Soft" singleton global object defined here,
//  but defined as extern and initialized in led-device.h.
LedMatrixDevice ledMatrixDevice;

void LedMatrixDevice::setup() { _matrix.begin(); }

void LedMatrixDevice::switchOn() {
  for (int r = 0; r < 8; r++) {
    for (int c = 0; c < 12; c++) {
      _frame[r][c] = 1;
    }
  }
  _matrix.renderBitmap(_frame, 8, 12);
}

void LedMatrixDevice::switchOff() {
  for (int r = 0; r < 8; r++) {
    for (int c = 0; c < 12; c++) {
      _frame[r][c] = 0;
    }
  }
  _matrix.renderBitmap(_frame, 8, 12);
}
}  // namespace myproject
