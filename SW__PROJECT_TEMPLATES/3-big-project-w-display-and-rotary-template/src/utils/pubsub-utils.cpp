#include "utils/pubsub-utils.h"

namespace myproject {
namespace pubsub_utils {

// "Soft" singleton global object defined here,
//  but defined as extern and initialized in pubsub-utils.h.
PubSub pubSub;

void PubSub::publish(ErrorStatusUpdateEvent event) {
  for (auto& callback : _errorStatusUpdateSubCallbacks) {
    callback(event);
  }
}

void PubSub::publish(AnyRotaryHoldEvent event) {
  for (auto& callback : _anyRotaryHoldSubCallbacks) {
    callback(event);
  }
}

void PubSub::publish(AllRotariesHoldEvent event) {
  for (auto& callback : _allRotariesHoldSubCallbacks) {
    callback(event);
  }
}

void PubSub::publish(TimerRotaryPressEvent event) {
  for (auto& callback : _timerRotaryPressSubCallbacks) {
    callback(event);
  }
}

void PubSub::publish(TimerRotaryRotationEvent event) {
  for (auto& callback : _timerRotaryRotationSubCallbacks) {
    callback(event);
  }
}

void PubSub::publish(TimerStartEvent event) {
  for (auto& callback : _timerStartSubCallbacks) {
    callback(event);
  }
}

void PubSub::publish(TimerEndEvent event) {
  for (auto& callback : _timerEndSubCallbacks) {
    callback(event);
  }
}

void PubSub::publish(TimerUpdateEvent event) {
  for (auto& callback : _timerUpdateSubCallbacks) {
    callback(event);
  }
}

void PubSub::subscribe(std::function<void(ErrorStatusUpdateEvent)> callback) {
  _errorStatusUpdateSubCallbacks.push_back(callback);
}

void PubSub::subscribe(std::function<void(AnyRotaryHoldEvent)> callback) {
  _anyRotaryHoldSubCallbacks.push_back(callback);
}

void PubSub::subscribe(std::function<void(AllRotariesHoldEvent)> callback) {
  _allRotariesHoldSubCallbacks.push_back(callback);
}

void PubSub::subscribe(std::function<void(TimerRotaryPressEvent)> callback) {
  _timerRotaryPressSubCallbacks.push_back(callback);
}

void PubSub::subscribe(std::function<void(TimerRotaryRotationEvent)> callback) {
  _timerRotaryRotationSubCallbacks.push_back(callback);
}

void PubSub::subscribe(std::function<void(TimerStartEvent)> callback) {
  _timerStartSubCallbacks.push_back(callback);
}

void PubSub::subscribe(std::function<void(TimerEndEvent)> callback) {
  _timerEndSubCallbacks.push_back(callback);
}

void PubSub::subscribe(std::function<void(TimerUpdateEvent)> callback) {
  _timerUpdateSubCallbacks.push_back(callback);
}

}  // namespace pubsub_utils
}  // namespace myproject
