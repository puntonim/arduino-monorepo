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
  // SUBSCRIPTIONS.
  pubsub_utils::pubSub.subscribe([this](
                                     pubsub_utils::NewScheduleEvent* pEvent) {
#if IS_DEBUG == true
    Serial.println((String) "HeatingDomain - received event: " + pEvent->topic);
#endif
    this->_onNewScheduleEvent();
  });

  pubsub_utils::pubSub.subscribe([this](
                                     pubsub_utils::AnyRotaryHoldEvent* pEvent) {
#if IS_DEBUG == true
    Serial.println((String) "HeatingDomain - received event: " + pEvent->topic);
#endif
    this->_stop();
  });

  // When rotating the timer rotary encoder we want the runCheck() to run
  //  asap (if the display is ON and there is an ongoing schedule) so the heater
  //  can turn ON/OFF instantly.
  pubsub_utils::pubSub.subscribe(
      [this](pubsub_utils::TimerRotaryChangeEvent* pEvent) {
#if IS_DEBUG == true
        Serial.println((String) "HeatingDomain - received event: " +
                       pEvent->topic);
#endif
        if (pEvent->isDisplayOn && schedulerDomain.isScheduled())
          this->runCheck();
      });

  // When rotating the target T rotary encoder we want the runCheck() to run
  //  asap (if the display is ON and there is an ongoing schedule) so the heater
  //  can turn ON/OFF instantly.
  pubsub_utils::pubSub.subscribe(
      [this](pubsub_utils::TargetTRotaryChangeEvent* pEvent) {
#if IS_DEBUG == true
        Serial.println((String) "HeatingDomain - received event: " +
                       pEvent->topic);
#endif
        if (pEvent->isDisplayOn && schedulerDomain.isScheduled())
          this->runCheck();
      });
}

void HeatingDomain::runCheck() {
#if IS_DEBUG == true
  Serial.println("HeatingDomain - run check");
#endif
  domainLedDevice.switchOn(true);

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

  if (!isOnForTime)
    _stop();
  else {
    if (isOnForTemp && (!_isOn))
      _switchOn();
    else if ((!isOnForTemp) && _isOn)
      _switchOff();
  }

  // TODO remove this useless delay.
  //  It's here otherwise the domainLed doesn't even blink as the heating domain
  //  is too fast.
  delay(100);

  domainLedDevice.switchOff(true);
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

void HeatingDomain::_onNewScheduleEvent() {
  // Execute `runCheck` so it reacts asap. Without this
  //  it would run after the period set in scheduleFixedRate, so 1 sec.
  runCheck();
#if IS_DEBUG == true
  Serial.println("HeatingDomain - starting a new run task");
#endif
  if (runTaskId != TASKMGR_INVALIDID) {
    // BUG if this happens then we run one more runTaskId every time we press
    //  the timer or temperature button. This will result in a freeze as
    //  the memory and cpu gets full of tasks.
#if IS_DEBUG == true
    Serial.println(
        "!!BUG!! HeatingDomain - starting a new run task but there is"
        " already at least 1 existing");
#endif
  } else
    runTaskId = taskManager.scheduleFixedRate(
        settings::HEATING_DOMAIN_RUN_PERIOD, [] { heatingDomain.runCheck(); });
}

/**
 * Stop: switch off the heating and cancel the ongoing schedule.
 */
void HeatingDomain::_stop() {
  _switchOff();
  schedulerDomain.reset();
  task_manager_utils::cancelTask(runTaskId);
}

}  // namespace tstat
