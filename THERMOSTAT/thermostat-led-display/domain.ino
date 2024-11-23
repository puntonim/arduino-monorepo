void Domain::setup() {
  // Set the callback function on the button press to switch on the display.
  buttonDevice.setOnPressCallback([&displayDevice] { displayDevice.switchOn(); });
  
  std::shared_ptr<bool> _isHeatingOnPointer(&_isHeatingOn);
  // Pass `_isHeatingOnPointer` to all devices that needs it (dependency injection).
  displayDevice.setIsHeatingOnPointer(_isHeatingOnPointer);
  heatingLedDevice.setIsHeatingOnPointer(_isHeatingOnPointer);

  // Run `checkPeriodically` so it reacts asap after boot. Without this
  //  it would run after the period set in scheduleFixedRate, so 1 sec.
  run();
  runTaskId = taskManager.scheduleFixedRate(1000, [] {
    domain.run();
  });
}

void Domain::run() {
  _checkTemperature();

  ///////_checkTime(); // TODO implement this.
}

void Domain::_checkTemperature() {
  Ds18b20SensorException exc;
  float temp = ds18b20Sensor.getData(exc);
  if (exc != Ds18b20SensorException::Success) {
    errorManager.addDs18b20SensorError();
  } else {
    errorManager.removeDs18b20SensorError();
  }

  if (temp < settings.TARGET_T) _switchHeatingOn();
  else _switchHeatingOff();
}

void Domain::_switchHeatingOn() {
  _isHeatingOn = true;
  // This is how we blink the heating LED (when heating is ON) with a slow
  //  period, the same period as run(): 1 sec.
  heatingLedDevice.autoUpdateStatus();
}

void Domain::_switchHeatingOff() {
  _isHeatingOn = false;
  heatingLedDevice.autoUpdateStatus();
}
