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

void PubSub::publish(AnyRotaryHoldEvent* pEvent) {
  for (auto& callback : _displayButtonHoldSubCallbacks) {
    callback(pEvent);
  }
}

void PubSub::publish(TargetTRotaryPressEvent* pEvent) {
  for (auto& callback : _targetTRotaryPressSubCallbacks) {
    callback(pEvent);
  }
}

void PubSub::publish(TargetTRotaryChangeEvent* pEvent) {
  for (auto& callback : _targetTRotaryChangeSubCallbacks) {
    callback(pEvent);
  }
}

void PubSub::publish(TimerRotaryPressEvent* pEvent) {
  for (auto& callback : _timerRotaryPressSubCallbacks) {
    callback(pEvent);
  }
}

void PubSub::publish(TimerRotaryChangeEvent* pEvent) {
  for (auto& callback : _timerRotaryChangeSubCallbacks) {
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

void PubSub::publish(NewScheduleEvent* pEvent) {
  for (auto& callback : _newScheduleSubCallbacks) {
    callback(pEvent);
  }
}

void PubSub::publish(SchedulerEditTimeEvent* pEvent) {
  for (auto& callback : _schedulerEditTimeSubCallbacks) {
    callback(pEvent);
  }
}

void PubSub::publish(SchedulerEditTargetTEvent* pEvent) {
  for (auto& callback : _schedulerEditTargetTSubCallbacks) {
    callback(pEvent);
  }
}

void PubSub::subscribe(std::function<void(DisplayButtonPressEvent*)> callback) {
  _displayButtonPressSubCallbacks.push_back(callback);
}

void PubSub::subscribe(std::function<void(AnyRotaryHoldEvent*)> callback) {
  _displayButtonHoldSubCallbacks.push_back(callback);
}

void PubSub::subscribe(std::function<void(TargetTRotaryPressEvent*)> callback) {
  _targetTRotaryPressSubCallbacks.push_back(callback);
}

void PubSub::subscribe(
    std::function<void(TargetTRotaryChangeEvent*)> callback) {
  _targetTRotaryChangeSubCallbacks.push_back(callback);
}

void PubSub::subscribe(std::function<void(TimerRotaryPressEvent*)> callback) {
  _timerRotaryPressSubCallbacks.push_back(callback);
}

void PubSub::subscribe(std::function<void(TimerRotaryChangeEvent*)> callback) {
  _timerRotaryChangeSubCallbacks.push_back(callback);
}

void PubSub::subscribe(
    std::function<void(HeatingStatusChangeEvent*)> callback) {
  _heatingStatusChangeSubCallbacks.push_back(callback);
}

void PubSub::subscribe(std::function<void(ErrorStatusChangeEvent*)> callback) {
  _errorStatusChangeSubCallbacks.push_back(callback);
}

void PubSub::subscribe(std::function<void(NewScheduleEvent*)> callback) {
  _newScheduleSubCallbacks.push_back(callback);
}

void PubSub::subscribe(std::function<void(SchedulerEditTimeEvent*)> callback) {
  _schedulerEditTimeSubCallbacks.push_back(callback);
}

void PubSub::subscribe(
    std::function<void(SchedulerEditTargetTEvent*)> callback) {
  _schedulerEditTargetTSubCallbacks.push_back(callback);
}

}  // namespace pubsub_utils
}  // namespace tstat
