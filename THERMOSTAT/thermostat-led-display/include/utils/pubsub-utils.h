#ifndef _PUBSUB_UTILS_H
#define _PUBSUB_UTILS_H

#include <functional>
#include <list>

#include "utils/time-utils.h"

namespace tstat {
namespace pubsub_utils {

class BasePubSubEvent {
 public:
  constexpr static char topic[] = "BASE_EVENT";

 private:
};

class DisplayButtonPressEvent : public BasePubSubEvent {
 public:
  constexpr static char topic[] = "DISPLAY_BUTTON_PRESS_EVENT";
};

class AnyRotaryHoldEvent : public BasePubSubEvent {
 public:
  constexpr static char topic[] = "ANY_ROTARY_HOLD_EVENT";
};

class TargetTRotaryPressEvent : public BasePubSubEvent {
 public:
  constexpr static char topic[] = "TARGET_T_ROTARY_PRESS_EVENT";
};

class TargetTRotaryChangeEvent : public BasePubSubEvent {
 public:
  constexpr static char topic[] = "TARGET_T_ROTARY_CHANGE_EVENT";
  const int16_t value;
  const bool isDisplayOn;
  TargetTRotaryChangeEvent(const int16_t value, const bool isDisplayOn)
      : value(value), isDisplayOn(isDisplayOn) {};
};

class TimerRotaryPressEvent : public BasePubSubEvent {
 public:
  constexpr static char topic[] = "TIMER_ROTARY_PRESS_EVENT";
};

class TimerRotaryChangeEvent : public BasePubSubEvent {
 public:
  constexpr static char topic[] = "TIMER_ROTARY_CHANGE_EVENT";
  const int16_t value;
  const bool isDisplayOn;
  TimerRotaryChangeEvent(const int16_t value, bool isDisplayOn)
      : value(value), isDisplayOn(isDisplayOn) {};
};

class HeatingStatusChangeEvent : public BasePubSubEvent {
 public:
  constexpr static char topic[] = "HEATING_STATUS_CHANGE_EVENT";
  const bool isOn;
  HeatingStatusChangeEvent(bool isOn) : isOn(isOn) {};
};

class ErrorStatusChangeEvent : public BasePubSubEvent {
 public:
  constexpr static char topic[] = "ERROR_STATUS_CHANGE_EVENT";
  const bool isError;
  std::list<char*> messageList;
  ErrorStatusChangeEvent(bool isError, std::list<char*> messageList = {})
      : isError(isError), messageList(messageList) {};
};

class NewScheduleEvent : public BasePubSubEvent {
 public:
  constexpr static char topic[] = "NEW_SCHEDULE_EVENT";
};

class SchedulerEditTimeEvent : public BasePubSubEvent {
 public:
  constexpr static char topic[] = "SCHEDULER_EDIT_TIME_EVENT";
  time_utils::Time time;
  SchedulerEditTimeEvent(time_utils::Time time) : time(time) {};
};

class SchedulerEditTargetTEvent : public BasePubSubEvent {
 public:
  constexpr static char topic[] = "SCHEDULER_EDIT_TARGET_T_EVENT";
  u_int8_t targetTemperature;
  SchedulerEditTargetTEvent(u_int8_t t) : targetTemperature(t) {};
};

class PubSub {
 private:
  std::list<std::function<void(DisplayButtonPressEvent*)>>
      _displayButtonPressSubCallbacks;
  std::list<std::function<void(AnyRotaryHoldEvent*)>>
      _displayButtonHoldSubCallbacks;
  std::list<std::function<void(TargetTRotaryPressEvent*)>>
      _targetTRotaryPressSubCallbacks;
  std::list<std::function<void(TargetTRotaryChangeEvent*)>>
      _targetTRotaryChangeSubCallbacks;
  std::list<std::function<void(TimerRotaryPressEvent*)>>
      _timerRotaryPressSubCallbacks;
  std::list<std::function<void(TimerRotaryChangeEvent*)>>
      _timerRotaryChangeSubCallbacks;
  std::list<std::function<void(HeatingStatusChangeEvent*)>>
      _heatingStatusChangeSubCallbacks;
  std::list<std::function<void(ErrorStatusChangeEvent*)>>
      _errorStatusChangeSubCallbacks;
  std::list<std::function<void(NewScheduleEvent*)>> _newScheduleSubCallbacks;
  std::list<std::function<void(SchedulerEditTimeEvent*)>>
      _schedulerEditTimeSubCallbacks;
  std::list<std::function<void(SchedulerEditTargetTEvent*)>>
      _schedulerEditTargetTSubCallbacks;

 public:
  void publish(DisplayButtonPressEvent* pEvent);
  void publish(AnyRotaryHoldEvent* pEvent);
  void publish(TargetTRotaryPressEvent* pEvent);
  void publish(TargetTRotaryChangeEvent* pEvent);
  void publish(TimerRotaryPressEvent* pEvent);
  void publish(TimerRotaryChangeEvent* pEvent);
  void publish(HeatingStatusChangeEvent* pEvent);
  void publish(ErrorStatusChangeEvent* pEvent);
  void publish(NewScheduleEvent* pEvent);
  void publish(SchedulerEditTimeEvent* pEvent);
  void publish(SchedulerEditTargetTEvent* pEvent);

  void subscribe(std::function<void(DisplayButtonPressEvent*)> callback);
  void subscribe(std::function<void(AnyRotaryHoldEvent*)> callback);
  void subscribe(std::function<void(TargetTRotaryPressEvent*)> callback);
  void subscribe(std::function<void(TargetTRotaryChangeEvent*)> callback);
  void subscribe(std::function<void(TimerRotaryPressEvent*)> callback);
  void subscribe(std::function<void(TimerRotaryChangeEvent*)> callback);
  void subscribe(std::function<void(HeatingStatusChangeEvent*)> callback);
  void subscribe(std::function<void(ErrorStatusChangeEvent*)> callback);
  void subscribe(std::function<void(NewScheduleEvent*)> callback);
  void subscribe(std::function<void(SchedulerEditTimeEvent*)> callback);
  void subscribe(std::function<void(SchedulerEditTargetTEvent*)> callback);
};

// "Soft" singleton global object defined as extern and initialized here,
//  but also defined in pubsub-utils.cpp.
extern PubSub pubSub;

}  // namespace pubsub_utils
}  // namespace tstat
#endif
