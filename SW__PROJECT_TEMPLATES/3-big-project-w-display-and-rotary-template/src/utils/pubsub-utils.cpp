#include "utils/pubsub-utils.h"

namespace myproject {
namespace pubsub_utils {

// "Soft" singleton global object defined here,
//  but defined as extern and initialized in pubsub-utils.h.
PubSub pubSub;

void PubSub::publish(ErrorStatusUpdateEvent* pEvent) {
  for (auto& callback : _errorStatusUpdateSubCallbacks) {
    callback(pEvent);
  }
}

void PubSub::publish(AnyRotaryHoldEvent* pEvent) {
  for (auto& callback : _anyRotaryHoldSubCallbacks) {
    callback(pEvent);
  }
}

void PubSub::publish(AllRotariesHoldEvent* pEvent) {
  for (auto& callback : _allRotariesHoldSubCallbacks) {
    callback(pEvent);
  }
}

void PubSub::publish(TimerRotaryPressEvent* pEvent) {
  for (auto& callback : _timerRotaryPressSubCallbacks) {
    callback(pEvent);
  }
}

void PubSub::publish(TimerRotaryRotationEvent* pEvent) {
  for (auto& callback : _timerRotaryRotationSubCallbacks) {
    callback(pEvent);
  }
}

void PubSub::publish(TimerStartEvent* pEvent) {
  for (auto& callback : _timerStartSubCallbacks) {
    callback(pEvent);
  }
}

void PubSub::publish(TimerEndEvent* pEvent) {
  for (auto& callback : _timerEndSubCallbacks) {
    callback(pEvent);
  }
}

void PubSub::publish(TimerUpdateEvent* pEvent) {
  for (auto& callback : _timerUpdateSubCallbacks) {
    callback(pEvent);
  }
}

void PubSub::subscribe(std::function<void(ErrorStatusUpdateEvent*)> callback) {
  _errorStatusUpdateSubCallbacks.push_back(callback);
}

void PubSub::subscribe(std::function<void(AnyRotaryHoldEvent*)> callback) {
  _anyRotaryHoldSubCallbacks.push_back(callback);
}

void PubSub::subscribe(std::function<void(AllRotariesHoldEvent*)> callback) {
  _allRotariesHoldSubCallbacks.push_back(callback);
}

void PubSub::subscribe(std::function<void(TimerRotaryPressEvent*)> callback) {
  _timerRotaryPressSubCallbacks.push_back(callback);
}

void PubSub::subscribe(
    std::function<void(TimerRotaryRotationEvent*)> callback) {
  _timerRotaryRotationSubCallbacks.push_back(callback);
}

void PubSub::subscribe(std::function<void(TimerStartEvent*)> callback) {
  _timerStartSubCallbacks.push_back(callback);
}

void PubSub::subscribe(std::function<void(TimerEndEvent*)> callback) {
  _timerEndSubCallbacks.push_back(callback);
}

void PubSub::subscribe(std::function<void(TimerUpdateEvent*)> callback) {
  _timerUpdateSubCallbacks.push_back(callback);
}

}  // namespace pubsub_utils
}  // namespace myproject
