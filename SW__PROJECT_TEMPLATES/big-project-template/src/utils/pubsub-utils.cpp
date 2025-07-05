#include "utils/pubsub-utils.h"

namespace bigpjtemplate {
namespace pubsub_utils {

// "Soft" singleton global object defined here,
//  but defined as extern and initialized in pubsub-utils.h.
PubSub pubSub;

void PubSub::publish(TimerStartEvent* pEvent) {
  for (auto& callback : _timerStartSubCallbacks) {
    callback(pEvent);
  }
}

void PubSub::publish(TimerFinishEvent* pEvent) {
  for (auto& callback : _timerFinishSubCallbacks) {
    callback(pEvent);
  }
}

void PubSub::subscribe(std::function<void(TimerStartEvent*)> callback) {
  _timerStartSubCallbacks.push_back(callback);
}

void PubSub::subscribe(std::function<void(TimerFinishEvent*)> callback) {
  _timerFinishSubCallbacks.push_back(callback);
}

}  // namespace pubsub_utils
}  // namespace bigpjtemplate
