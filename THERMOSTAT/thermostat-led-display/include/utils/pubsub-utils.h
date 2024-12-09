#ifndef _PUBSUB_UTILS_H
#define _PUBSUB_UTILS_H

#include <functional>
#include <list>

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

class TargetTButtonPressEvent : public BasePubSubEvent {
 public:
  constexpr static char topic[] = "TARGET_T_BUTTON_PRESS_EVENT";
};

class TimerButtonPressEvent : public BasePubSubEvent {
 public:
  constexpr static char topic[] = "TIMER_BUTTON_PRESS_EVENT";
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

class PubSub {
 private:
  std::list<std::function<void(DisplayButtonPressEvent*)>>
      _displayButtonPressSubCallbacks;
  std::list<std::function<void(TargetTButtonPressEvent*)>>
      _targetTButtonPressSubCallbacks;
  std::list<std::function<void(TimerButtonPressEvent*)>>
      _timerButtonPressSubCallbacks;
  std::list<std::function<void(HeatingStatusChangeEvent*)>>
      _heatingStatusChangeSubCallbacks;
  std::list<std::function<void(ErrorStatusChangeEvent*)>>
      _errorStatusChangeSubCallbacks;

 public:
  void publish(DisplayButtonPressEvent* pEvent);
  void publish(TargetTButtonPressEvent* pEvent);
  void publish(TimerButtonPressEvent* pEvent);
  void publish(HeatingStatusChangeEvent* pEvent);
  void publish(ErrorStatusChangeEvent* pEvent);

  void subscribe(std::function<void(DisplayButtonPressEvent*)> callback);
  void subscribe(std::function<void(TargetTButtonPressEvent*)> callback);
  void subscribe(std::function<void(TimerButtonPressEvent*)> callback);
  void subscribe(std::function<void(HeatingStatusChangeEvent*)> callback);
  void subscribe(std::function<void(ErrorStatusChangeEvent*)> callback);
};

// "Soft" singleton global object defined as extern and initialized here,
//  but also defined in pubsub-utils.cpp.
extern PubSub pubSub;

}  // namespace pubsub_utils
}  // namespace tstat
#endif
