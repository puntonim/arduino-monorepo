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

class ButtonPressEvent : public BasePubSubEvent {
 public:
  constexpr static char topic[] = "BUTTON_PRESS_EVENT";
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
  std::list<std::function<void(ButtonPressEvent*)>> _buttonPressSubCallbacks;
  std::list<std::function<void(HeatingStatusChangeEvent*)>>
      _heatingStatusChangeSubCallbacks;
  std::list<std::function<void(ErrorStatusChangeEvent*)>>
      _errorStatusChangeSubCallbacks;

 public:
  void publish(ButtonPressEvent* pEvent);
  void publish(HeatingStatusChangeEvent* pEvent);
  void publish(ErrorStatusChangeEvent* pEvent);

  void subscribe(std::function<void(ButtonPressEvent*)> callback);
  void subscribe(std::function<void(HeatingStatusChangeEvent*)> callback);
  void subscribe(std::function<void(ErrorStatusChangeEvent*)> callback);
};

// "Soft" singleton global object defined as extern and initialized here,
//  but also defined in pubsub-utils.cpp.
extern PubSub pubSub;

}  // namespace pubsub_utils
}  // namespace tstat
#endif
