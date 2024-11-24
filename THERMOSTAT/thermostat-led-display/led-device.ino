//********** CLASS HeatingLedDevice *****************************************************

void HeatingLedDevice::setup() {
  pinMode(settings.HEATING_LED_PIN, OUTPUT);

  pubSub.subscribe([](HeatingOnEvent* pEvent) {
    heatingLedDevice.startBlinking();
  });  
  pubSub.subscribe([](HeatingOffEvent* pEvent) {
    heatingLedDevice.stopBlinking();
  });  
}

void HeatingLedDevice::switchOn() {
  analogWrite(settings.HEATING_LED_PIN, settings.HEATING_LED_BRIGHTNESS_VALUE);
  _isOn = true;
}

void HeatingLedDevice::switchOff() {
  analogWrite(settings.HEATING_LED_PIN, 0);
  _isOn = false;
}

void HeatingLedDevice::toggle() {
  if (_isOn) switchOff();
  else switchOn();
}

void HeatingLedDevice::startBlinking() {
  // Start the blinking task only if it's not already running.
  if (blinkHeatingLedTaskId == TASKMGR_INVALIDID) {
    blinkHeatingLedTaskId = taskManager.scheduleFixedRate(1000, [] {
      heatingLedDevice.toggle();
    });
  }
}

void HeatingLedDevice::stopBlinking() {
  cancelTask(blinkHeatingLedTaskId);
  // Make sure the LED is off as the task might have left it on.
  switchOff();
}


//********** CLASS ErrorLedDevice *******************************************************

void ErrorLedDevice::setup() {
  pinMode(settings.ERROR_LED_PIN, OUTPUT);

  pubSub.subscribe([](ErrorStateEvent* pEvent) {
    errorLedDevice.startBlinking();
  });
  pubSub.subscribe([](NoErrorStateEvent* pEvent) {
    errorLedDevice.stopBlinking();
  });
}

void ErrorLedDevice::switchOn() {
  analogWrite(settings.ERROR_LED_PIN, settings.ERROR_LED_BRIGHTNESS_VALUE);
  _isOn = true;
}

void ErrorLedDevice::switchOff() {
  analogWrite(settings.ERROR_LED_PIN, 0);
  _isOn = false;
}

void ErrorLedDevice::toggle() {
  _isOn ? switchOff() : switchOn();
}

void ErrorLedDevice::startBlinking() {
  // Start the blinking task only if it's not already running.
  if (blinkErrorLedTaskId == TASKMGR_INVALIDID) {
    blinkErrorLedTaskId = taskManager.scheduleFixedRate(100, [] {
      errorLedDevice.toggle();
    });
  }
}

void ErrorLedDevice::stopBlinking() {
  cancelTask(blinkErrorLedTaskId);
  // Make sure the led is off as the task might have left it on.
  switchOff();
}
