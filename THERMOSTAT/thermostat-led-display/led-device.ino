//********** CLASS HeatingLedDevice ***************************************************

void HeatingLedDevice::setup() {
  pinMode(_PIN, OUTPUT);

  pubSub.subscribe([this](HeatingOnEvent* pEvent) {
#if IS_DEBUG == true
    Serial.println((String) "HeatingLedDevice - received event: " + pEvent->topic);
#endif
    this->startBlinking();
  });
  pubSub.subscribe([this](HeatingOffEvent* pEvent) {
#if IS_DEBUG == true
    Serial.println((String) "HeatingLedDevice - received event: " + pEvent->topic);
#endif
    this->stopBlinking();
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

void ErrorLedDevice::setup() {
  pinMode(_PIN, OUTPUT);

  pubSub.subscribe([this](ErrorStateEvent* pEvent) {
#if IS_DEBUG == true
    Serial.println((String) "ErrorLedDevice - received event: " + pEvent->topic);
#endif
    this->startBlinking();
  });
  pubSub.subscribe([this](NoErrorStateEvent* pEvent) {
#if IS_DEBUG == true
    Serial.println((String) "ErrorLedDevice - received event: " + pEvent->topic);
#endif
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
  analogWrite(_PIN, _BRIGHTNESS_VALUE);
  _isOn = true;
}

void DomainLedDevice::switchOff() {
  analogWrite(_PIN, 0);
  _isOn = false;
}