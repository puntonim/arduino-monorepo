void Domain::setup() {
  // Run `checkPeriodically` so it reacts asap after boot. Without this
  //  it would run after the period set in scheduleFixedRate, so 1 sec.
  run();
#if IS_DEBUG == true
  Serial.println((String) "Domain - starting a new run task");
#endif
  runTaskId = taskManager.scheduleFixedRate(settings.DOMAIN_RUN_PERIOD, [] {
    domain.run();
  });

  timer.start(settings.DEFAULT_TIMER.h, settings.DEFAULT_TIMER.m, settings.DEFAULT_TIMER.s);
}

void Domain::run() {
  domainLedDevice.switchOn();
  _checkTemperature();

  // _checkTime(); // TODO implement this.

  // TODO remove this useless delay.
  //  It's here otherwise the domainLed doesn't even blink as the domain is too fast.
  delay(100);

  domainLedDevice.switchOff();
}

void Domain::_checkTemperature() {
  Ds18b20SensorException exc;
  // Reading only DS18B20 temp for now (and ignoring SHT85 sensor).
  float sensorTemp = ds18b20Sensor.getData(exc);
  // if (exc == Ds18b20SensorException::SensorError) ...

  // TODO replace this with the PID algo: https://playground.arduino.cc/Code/PIDLibrary/
  if ((sensorTemp < settings.TARGET_T) && !_isHeatingOn) _switchHeatingOn();
  else if ((sensorTemp >= settings.TARGET_T) && _isHeatingOn) _switchHeatingOff();
}

void Domain::_switchHeatingOn() {
  _isHeatingOn = true;

  pubSub.publish(new HeatingStatusChangeEvent(true));
}

void Domain::_switchHeatingOff() {
  _isHeatingOn = false;

  pubSub.publish(new HeatingStatusChangeEvent(false));
}
