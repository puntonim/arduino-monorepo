#include "utils/pubsub-utils.h"

namespace tstat {
namespace pubsub_utils {

// "Soft" singleton global object defined here,
//  but defined as extern and initialized in pubsub-utils.h.
PubSub pubSub;

void PubSub::publish(ButtonPressEvent* pEvent) {
  for (auto& callback : _buttonPressSubCallbacks) {
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

void PubSub::subscribe(std::function<void(ButtonPressEvent*)> callback) {
  _buttonPressSubCallbacks.push_back(callback);
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
