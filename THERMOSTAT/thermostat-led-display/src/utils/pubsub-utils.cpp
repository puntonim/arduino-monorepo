#include "utils/pubsub-utils.h"

namespace tstat {
namespace pubsub_utils {

// "Soft" singleton global object defined here,
//  but defined as extern and initialized in pubsub-utils.h.
PubSub pubSub;

void PubSub::publish(DisplayButtonPressEvent* pEvent) {
  for (auto& callback : _displayButtonPressSubCallbacks) {
    callback(pEvent);
  }
}

void PubSub::publish(TargetTButtonPressEvent* pEvent) {
  for (auto& callback : _targetTButtonPressSubCallbacks) {
    callback(pEvent);
  }
}

void PubSub::publish(TimerButtonPressEvent* pEvent) {
  for (auto& callback : _timerButtonPressSubCallbacks) {
    callback(pEvent);
  }
}

void PubSub::publish(HeatingStatusChangeEvent* pEvent) {
  for (auto& callback : _heatingStatusChangeSubCallbacks) {
    callback(pEvent);
  }
}

void PubSub::publish(ErrorStatusChangeEvent* pEvent) {
  for (auto& callback : _errorStatusChangeSubCallbacks) {
    callback(pEvent);
  }
}

void PubSub::subscribe(std::function<void(DisplayButtonPressEvent*)> callback) {
  _displayButtonPressSubCallbacks.push_back(callback);
}

void PubSub::subscribe(std::function<void(TargetTButtonPressEvent*)> callback) {
  _targetTButtonPressSubCallbacks.push_back(callback);
}

void PubSub::subscribe(std::function<void(TimerButtonPressEvent*)> callback) {
  _timerButtonPressSubCallbacks.push_back(callback);
}

void PubSub::subscribe(
    std::function<void(HeatingStatusChangeEvent*)> callback) {
  _heatingStatusChangeSubCallbacks.push_back(callback);
}

void PubSub::subscribe(std::function<void(ErrorStatusChangeEvent*)> callback) {
  _errorStatusChangeSubCallbacks.push_back(callback);
}

}  // namespace pubsub_utils
}  // namespace tstat
