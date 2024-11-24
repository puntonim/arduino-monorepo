//********** CLASS HeatingLedDevice ***************************************************

void HeatingLedDevice::setup() {
  pinMode(_PIN, OUTPUT);

  pubSub.subscribe([this](HeatingOnEvent* pEvent) {
    this->startBlinking();
  });  
  pubSub.subscribe([this](HeatingOffEvent* pEvent) {
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


//********** CLASS ErrorLedDevice *****************************************************

void ErrorLedDevice::setup() {
  pinMode(_PIN, OUTPUT);

  pubSub.subscribe([this](ErrorStateEvent* pEvent) {
    this->startBlinking();
  });
  pubSub.subscribe([this](NoErrorStateEvent* pEvent) {
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