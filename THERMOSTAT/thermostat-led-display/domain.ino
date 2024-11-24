void Domain::setup() {
  // Run `checkPeriodically` so it reacts asap after boot. Without this
  //  it would run after the period set in scheduleFixedRate, so 1 sec.
  run();
  runTaskId = taskManager.scheduleFixedRate(1000, [] {
    domain.run();
  });
}

void Domain::run() {
  domainLedDevice.switchOn();
  _checkTemperature();

  // _checkTime(); // TODO implement this.
  domainLedDevice.switchOff();
}

void Domain::_checkTemperature() {
  Ds18b20SensorException exc;
  float temp = ds18b20Sensor.getData(exc);
  if (exc != Ds18b20SensorException::Success) {
    errorManager.addDs18b20SensorError();
    // TODO publish error event.
  } else {
    errorManager.removeDs18b20SensorError();
  }

  if (temp < settings.TARGET_T) _switchHeatingOn();
  else _switchHeatingOff();
}

void Domain::_switchHeatingOn() {
  _isHeatingOn = true;

  pubSub.publish(new HeatingOnEvent());
}

void Domain::_switchHeatingOff() {
  _isHeatingOn = false;

  pubSub.publish(new HeatingOffEvent());
}
