void Domain::setup() {
  // TODO this should be done with a rotary encoder.
  timer.start(settings.DEFAULT_TIMER.h, settings.DEFAULT_TIMER.m, settings.DEFAULT_TIMER.s);

  // Run `checkPeriodically` so it reacts asap after boot. Without this
  //  it would run after the period set in scheduleFixedRate, so 1 sec.
  run();
#if IS_DEBUG == true
  Serial.println((String) "Domain - starting a new run task");
#endif
  runTaskId = taskManager.scheduleFixedRate(settings.DOMAIN_RUN_PERIOD, [] {
    domain.run();
  });
}

void Domain::run() {
  domainLedDevice.switchOn();
  
  bool isOnForTemp = _checkForTemperature();
  bool isOnForTime = _checkForTimer();

  // -------------- DECISION TABLE -------------------
  // isOnForTime   isOnForTemp   status     action to take
  // -----------   -----------   ------     --------------
  //     F           T or F        F     -> noop
  //                               T     -> switch OFF
  //     T             T           T     -> noop
  //                               F     -> switch ON
  //     T             F           F     -> noop
  //                               T     -> switch OFF
  //
  // Notes:
  //  - `isOnForTime` is true when the timer is not over yet;
  //  - `isOnForTemp` is true when the sensor T is < target T
  //  - `status` is true when the heating is ON;

  if ((!isOnForTime) && _isHeatingOn) _switchHeatingOff();
  else if (isOnForTime && isOnForTemp && (!_isHeatingOn)) _switchHeatingOn();
  else if (isOnForTime && (!isOnForTemp) && _isHeatingOn) _switchHeatingOff();

  // TODO remove this useless delay.
  //  It's here otherwise the domainLed doesn't even blink as the domain is too fast.
  delay(100);

  domainLedDevice.switchOff();
}

bool Domain::_checkForTemperature() {
  Ds18b20SensorException exc;
  // Reading only DS18B20 temp for now (and ignoring SHT85 sensor).
  float sensorTemp = ds18b20Sensor.getData(exc);
  // if (exc == Ds18b20SensorException::SensorError) ...

  // TODO replace this with the PID algo: https://playground.arduino.cc/Code/PIDLibrary/
  // if ((sensorTemp < settings.TARGET_T) && !_isHeatingOn) _switchHeatingOn();
  // else if ((sensorTemp >= settings.TARGET_T) && _isHeatingOn) _switchHeatingOff();
  return sensorTemp < settings.TARGET_T;
}

bool Domain::_checkForTimer() {
  timer.tick();
  return !timer.isOver();
}

void Domain::_switchHeatingOn() {
  _isHeatingOn = true;

  pubSub.publish(new pubsub_utils::HeatingStatusChangeEvent(true));
}

void Domain::_switchHeatingOff() {
  _isHeatingOn = false;

  pubSub.publish(new pubsub_utils::HeatingStatusChangeEvent(false));
}
