#ifndef _PUBSUB_UTILS_H
#define _PUBSUB_UTILS_H

#include <functional>
#include <list>

#include "utils/time-utils.h"

namespace bigpjtemplate {
namespace pubsub_utils {

class BasePubSubEvent {
 public:
  constexpr static char topic[] = "BASE_EVENT";

 private:
};

class ErrorStatusUpdateEvent : public BasePubSubEvent {
 public:
  constexpr static char topic[] = "ERROR_STATUS_UPDATE_EVENT";
  const bool isError;
  ErrorStatusUpdateEvent(bool isError) : isError(isError) {};
};

class AnyRotaryHoldEvent : public BasePubSubEvent {
 public:
  constexpr static char topic[] = "ANY_ROTARY_HOLD_EVENT";
};

class AllRotariesHoldEvent : public BasePubSubEvent {
 public:
  constexpr static char topic[] = "ALL_ROTARIES_HOLD_EVENT";
};

class TimerRotaryPressEvent : public BasePubSubEvent {
 public:
  constexpr static char topic[] = "TIMER_ROTARY_PRESS_EVENT";
};

class TimerRotaryRotationEvent : public BasePubSubEvent {
 public:
  constexpr static char topic[] = "TIMER_ROTARY_CHANGE_EVENT";
  const int16_t value;
  const bool isDisplayOn;
  TimerRotaryRotationEvent(const int16_t value, bool isDisplayOn)
      : value(value), isDisplayOn(isDisplayOn) {};
};

class TimerStartEvent : public BasePubSubEvent {
 public:
  constexpr static char topic[] = "TIMER_START_EVENT";
};

class TimerEndEvent : public BasePubSubEvent {
 public:
  constexpr static char topic[] = "TIMER_END_EVENT";
};

class TimerUpdateEvent : public BasePubSubEvent {
 public:
  constexpr static char topic[] = "TIMER_UPDATE_EVENT";
  time_utils::Time time;
  TimerUpdateEvent(time_utils::Time time) : time(time) {};
};

class PubSub {
 private:
  std::list<std::function<void(ErrorStatusUpdateEvent*)>>
      _errorStatusUpdateSubCallbacks;
  std::list<std::function<void(AnyRotaryHoldEvent*)>>
      _anyRotaryHoldSubCallbacks;
  std::list<std::function<void(AllRotariesHoldEvent*)>>
      _allRotariesHoldSubCallbacks;
  std::list<std::function<void(TimerRotaryPressEvent*)>>
      _timerRotaryPressSubCallbacks;
  std::list<std::function<void(TimerRotaryRotationEvent*)>>
      _timerRotaryChangeSubCallbacks;
  std::list<std::function<void(TimerStartEvent*)>> _timerStartSubCallbacks;
  std::list<std::function<void(TimerEndEvent*)>> _timerEndSubCallbacks;
  std::list<std::function<void(TimerUpdateEvent*)>> _timerUpdateSubCallbacks;

 public:
  void publish(ErrorStatusUpdateEvent* pEvent);
  void publish(AnyRotaryHoldEvent* pEvent);
  void publish(AllRotariesHoldEvent* pEvent);
  void publish(TimerRotaryPressEvent* pEvent);
  void publish(TimerRotaryRotationEvent* pEvent);
  void publish(TimerStartEvent* pEvent);
  void publish(TimerEndEvent* pEvent);
  void publish(TimerUpdateEvent* pEvent);

  void subscribe(std::function<void(ErrorStatusUpdateEvent*)> callback);
  void subscribe(std::function<void(AnyRotaryHoldEvent*)> callback);
  void subscribe(std::function<void(AllRotariesHoldEvent*)> callback);
  void subscribe(std::function<void(TimerRotaryPressEvent*)> callback);
  void subscribe(std::function<void(TimerRotaryRotationEvent*)> callback);
  void subscribe(std::function<void(TimerStartEvent*)> callback);
  void subscribe(std::function<void(TimerEndEvent*)> callback);
  void subscribe(std::function<void(TimerUpdateEvent*)> callback);
};

// "Soft" singleton global object defined as extern and initialized here,
//  but also defined in pubsub-utils.cpp.
extern PubSub pubSub;

}  // namespace pubsub_utils
}  // namespace bigpjtemplate
#endif
