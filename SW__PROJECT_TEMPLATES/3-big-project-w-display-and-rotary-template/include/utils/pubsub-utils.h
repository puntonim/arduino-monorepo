#ifndef _PUBSUB_UTILS_H
#define _PUBSUB_UTILS_H

#include <functional>
#include <list>

#include "devices/display-device.h"
#include "utils/time-utils.h"

namespace myproject {
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
  // `isError` is a constructor's arg because that info is available in the
  //  object (ErrorMgr) that publishes this event. Mind how different it is for
  //  TimerRotaryRotationEvent instead.
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
  constexpr static char topic[] = "TIMER_ROTARY_ROTATION_EVENT";
  const int16_t value;
  bool isDisplayOn;

  TimerRotaryRotationEvent(const int16_t value, const bool isDisplayOn)
      : value(value), isDisplayOn(isDisplayOn) {};

  // Note: having the attribute `isDisplayOn` would be handy, but this info
  //  is NOT available in the same object (rotary-encoder-device.cpp) that
  //  publishes this event. So I thought of setting the attribute in
  //  the constructor here, but that would mean including `display-device.h`
  //  here that could cause a circular dependency, as DisplayDevice might
  //  need to include `pubsub-utils.h`. So I commented out this code and I
  //  made the publisher of this event (TimerRotaryEncoder) aware of the
  //  status of the display by including `display-device.h` and using
  //  `displayDevice` directly. Notice that the same thing could not
  //  be done in the consumer (MainDomain) because it needs to know if the
  //  display was ON when the even was published not when it was received (as
  //  in the meantime the DispleyDevice might have turned it on).
  // bool isDisplayOn;
  // // `isDisplayOn` is NOT a constructor's arg because that info is NOT
  // //  available in the same object (rotary-encoder-device.cpp) that publishes
  // //  this event. So we include displayDevice here (rather than in that
  // object)
  // //  and get the info. This ws the code is better de-coupled. Mind how
  // //  different it is for ErrorStatusUpdateEvent instead.
  // TimerRotaryRotationEvent(const int16_t value) : value(value) {
  //   isDisplayOn = displayDevice.isOn();
  // };
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
  const time_utils::Time time;
  // `time` is a constructor's arg because that info is available in the
  //  object (MainDomain) that publishes this event. Mind how different it is
  //  for TimerRotaryRotationEvent instead.
  TimerUpdateEvent(time_utils::Time time) : time(time) {};
};

class PubSub {
 private:
  std::list<std::function<void(ErrorStatusUpdateEvent)>>
      _errorStatusUpdateSubCallbacks;
  std::list<std::function<void(AnyRotaryHoldEvent)>> _anyRotaryHoldSubCallbacks;
  std::list<std::function<void(AllRotariesHoldEvent)>>
      _allRotariesHoldSubCallbacks;
  std::list<std::function<void(TimerRotaryPressEvent)>>
      _timerRotaryPressSubCallbacks;
  std::list<std::function<void(TimerRotaryRotationEvent)>>
      _timerRotaryRotationSubCallbacks;
  std::list<std::function<void(TimerStartEvent)>> _timerStartSubCallbacks;
  std::list<std::function<void(TimerEndEvent)>> _timerEndSubCallbacks;
  std::list<std::function<void(TimerUpdateEvent)>> _timerUpdateSubCallbacks;

 public:
  void publish(ErrorStatusUpdateEvent event);
  void publish(AnyRotaryHoldEvent event);
  void publish(AllRotariesHoldEvent event);
  void publish(TimerRotaryPressEvent event);
  void publish(TimerRotaryRotationEvent event);
  void publish(TimerStartEvent event);
  void publish(TimerEndEvent event);
  void publish(TimerUpdateEvent event);

  void subscribe(std::function<void(ErrorStatusUpdateEvent)> callback);
  void subscribe(std::function<void(AnyRotaryHoldEvent)> callback);
  void subscribe(std::function<void(AllRotariesHoldEvent)> callback);
  void subscribe(std::function<void(TimerRotaryPressEvent)> callback);
  void subscribe(std::function<void(TimerRotaryRotationEvent)> callback);
  void subscribe(std::function<void(TimerStartEvent)> callback);
  void subscribe(std::function<void(TimerEndEvent)> callback);
  void subscribe(std::function<void(TimerUpdateEvent)> callback);
};

// "Soft" singleton global object defined as extern and initialized here,
//  but also defined in pubsub-utils.cpp.
extern PubSub pubSub;

}  // namespace pubsub_utils
}  // namespace myproject
#endif
