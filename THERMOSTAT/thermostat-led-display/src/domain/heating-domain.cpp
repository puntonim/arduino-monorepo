#include "domain/heating-domain.h"

#include <TaskManagerIO.h>

#include "devices/led-device.h"
#include "domain/scheduler-domain.h"
#include "sensors/ds18b20-sensor.h"
#include "sensors/sht85-sensor.h"
#include "settings.h"
#include "utils/pubsub-utils.h"
#include "utils/task-manager-utils.h"
#include "utils/time-utils.h"

namespace tstat {

// "Soft" singleton global object defined here,
//  but defined as extern and initialized in heating-domain.h.
HeatingDomain heatingDomain;

void HeatingDomain::setup() {
  // TODO this should be done with a rotary encoder.
  schedulerDomain.timer.start(settings::DEFAULT_TIMER.h,
                              settings::DEFAULT_TIMER.m,
                              settings::DEFAULT_TIMER.s);

  // Run `checkPeriodically` so it reacts asap after boot. Without this
  //  it would run after the period set in scheduleFixedRate, so 1 sec.
  run();
#if IS_DEBUG == true
  Serial.println((String) "HeatingDomain - starting a new run task");
#endif
  runTaskId = taskManager.scheduleFixedRate(settings::HEATING_DOMAIN_RUN_PERIOD,
                                            [] { heatingDomain.run(); });

  // TODO deleteme
  pubsub_utils::pubSub.subscribe(
      [this](pubsub_utils::TargetTButtonPressEvent* pEvent) {
        Serial.println((String) "TMP1 - received event: " + pEvent->topic);
      });
  pubsub_utils::pubSub.subscribe(
      [this](pubsub_utils::TimerButtonPressEvent* pEvent) {
        Serial.println((String) "TMP2 - received event: " + pEvent->topic);
      });
}

void HeatingDomain::run() {
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

  if (!isOnForTime) task_manager_utils::cancelTask(runTaskId);

  if ((!isOnForTime) && _isOn)
    _switchOff();
  else if (isOnForTime && isOnForTemp && (!_isOn))
    _switchOn();
  else if (isOnForTime && (!isOnForTemp) && _isOn)
    _switchOff();

  // TODO remove this useless delay.
  //  It's here otherwise the domainLed doesn't even blink as the heating domain
  //  is too fast.
  delay(100);

  domainLedDevice.switchOff();
}

bool HeatingDomain::_checkForTemperature() {
  Ds18b20SensorException exc;
  // Reading only DS18B20 temp for now (and ignoring SHT85 sensor).
  float sensorTemp = ds18b20Sensor.getData(exc);
  // There is no need to handle the exception because, in case of sensor error,
  //  a message is published and the subscribed callbacks are invoked.
  // if (exc == Ds18b20SensorException::SensorError)

  // TODO replace this with the PID algo:
  // https://playground.arduino.cc/Code/PIDLibrary/ if ((sensorTemp <
  // settings.TARGET_T) && !_isOn) _switchOn(); else if
  // ((sensorTemp >= settings.TARGET_T) && _isOn) _switchOff();
  return sensorTemp < schedulerDomain.targetTemperature;
}

bool HeatingDomain::_checkForTimer() {
  schedulerDomain.timer.tick();
  return !schedulerDomain.timer.isOver();
}

void HeatingDomain::_switchOn() {
  _isOn = true;

  pubsub_utils::pubSub.publish(
      new pubsub_utils::HeatingStatusChangeEvent(true));
}

void HeatingDomain::_switchOff() {
  _isOn = false;

  pubsub_utils::pubSub.publish(
      new pubsub_utils::HeatingStatusChangeEvent(false));
}

}  // namespace tstat
