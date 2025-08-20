#ifndef _PUBSUB_UTILS_H
#define _PUBSUB_UTILS_H

#include <functional>
#include <list>

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

class PubSub {
 private:
  std::list<std::function<void(ErrorStatusUpdateEvent*)>>
      _errorStatusUpdateSubCallbacks;

 public:
  void publish(ErrorStatusUpdateEvent* pEvent);

  void subscribe(std::function<void(ErrorStatusUpdateEvent*)> callback);
};

// "Soft" singleton global object defined as extern and initialized here,
//  but also defined in pubsub-utils.cpp.
extern PubSub pubSub;

}  // namespace pubsub_utils
}  // namespace bigpjtemplate
#endif
