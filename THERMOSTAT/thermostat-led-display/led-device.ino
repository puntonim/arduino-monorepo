//********** CLASS HeatingLedDevice *****************************************************

void HeatingLedDevice::setup() {
  pinMode(settings.HEATING_LED_PIN, OUTPUT);
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

void HeatingLedDevice::autoUpdateStatus() {
  if (*_isHeatingOnPointer == true) toggle();
  else switchOff();
}

// We need a slow blinking for the heating LED, so no need for the blinking functions but 
//  instead we manage the blinking in Domain::run() (and with its same slow period of 1 sec).
// void HeatingLedDevice::startBlinking() {}
// void HeatingLedDevice::stopBlinking() {}

void HeatingLedDevice::setIsHeatingOnPointer(std::shared_ptr<const bool> isHeatingOnPointer) {
  _isHeatingOnPointer = isHeatingOnPointer;
}


//********** CLASS ErrorLedDevice *******************************************************

void ErrorLedDevice::setup() {
  pinMode(settings.ERROR_LED_PIN, OUTPUT);
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

/**
 * We need an aggressive (fast) blinking for the error LED.
 * So we cannot do it in run() like for the heating LED, but
 *  instead we start a new task with a high frequency. It's not a waste as
 *  error conditions are exceptional (and rare hopefully).
 */
void ErrorLedDevice::startBlinking() {
  // Start the blinking task only if it's not already running.
  if (toggleErrorLedTaskId == TASKMGR_INVALIDID) {
    toggleErrorLedTaskId = taskManager.scheduleFixedRate(100, [] {
      errorLedDevice.toggle();
    });
  }
}

void ErrorLedDevice::stopBlinking() {
  cancelTask(toggleErrorLedTaskId);
  // Make sure it's off as toggleErrorLedTaskId() might have left it on.
  switchOff();
}