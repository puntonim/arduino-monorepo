//********** CLASS HeatingLedDevice ***************************************************

void HeatingLedDevice::setup() {
  pinMode(_PIN, OUTPUT);

  pubSub.subscribe([this](HeatingStatusChangeEvent* pEvent) {
#if IS_DEBUG == true
    Serial.println((String) "HeatingLedDevice - received event: " + pEvent->topic + " isOn=" + (pEvent->isOn ? "ON" : "OFF"));
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
  if (_isOn) switchOff();
  else switchOn();
}

void HeatingLedDevice::startBlinking() {
  // Start the blinking task only if it's not already running.
  if (blinkHeatingLedTaskId == TASKMGR_INVALIDID) {
#if IS_DEBUG == true
    Serial.println((String) "HeatingLedDevice - starting a new blinking task");
#endif
    blinkHeatingLedTaskId = taskManager.scheduleFixedRate(settings.HEATING_LED_BLINKING_PERIOD, [] {
      heatingLedDevice.toggle();
    });
  }
}

void HeatingLedDevice::stopBlinking() {
#if IS_DEBUG == true
  Serial.println((String) "HeatingLedDevice - stopping the blinking task");
#endif
  cancelTask(blinkHeatingLedTaskId);
  // Make sure the LED is off as the task might have left it on.
  switchOff();
}


//********** CLASS ErrorLedDevice *****************************************************

// Note: at the moment the sensor SHT85 is only read by the display, so its error status is updated
//  only when the display is ON. This means that if the errorLed has been triggered by an error on
//   the SHT85 sensor, then the errorLed status can be delayed until the display is ON again.

void ErrorLedDevice::setup() {
  pinMode(_PIN, OUTPUT);

  pubSub.subscribe([this](ErrorStatusChangeEvent* pEvent) {
#if IS_DEBUG == true
    Serial.println((String) "ErrorLedDevice - received event: " + pEvent->topic + " isError=" + (pEvent->isError ? "ON" : "OFF"));
#endif
    if (pEvent->isError) this->startBlinking();
    else this->stopBlinking();
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

void ErrorLedDevice::toggle() {
  _isOn ? switchOff() : switchOn();
}

void ErrorLedDevice::startBlinking() {
  // Start the blinking task only if it's not already running.
  if (blinkErrorLedTaskId == TASKMGR_INVALIDID) {
#if IS_DEBUG == true
    Serial.println((String) "ErrorLedDevice - starting a new blinking task");
#endif
    blinkErrorLedTaskId = taskManager.scheduleFixedRate(settings.ERROR_LED_BLINKING_PERIOD, [] {
      errorLedDevice.toggle();
    });
  }
}

void ErrorLedDevice::stopBlinking() {
#if IS_DEBUG == true
  Serial.println((String) "ErrorLedDevice - stopping the blinking task");
#endif
  cancelTask(blinkErrorLedTaskId);
  // Make sure the led is off as the task might have left it on.
  switchOff();
}


//********** CLASS DomainLedDevice ****************************************************

void DomainLedDevice::setup() {
  pinMode(_PIN, OUTPUT);
}

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