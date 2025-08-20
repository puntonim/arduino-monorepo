#include "utils/pubsub-utils.h"

namespace bigpjtemplate {
namespace pubsub_utils {

// "Soft" singleton global object defined here,
//  but defined as extern and initialized in pubsub-utils.h.
PubSub pubSub;

void PubSub::publish(ErrorStatusUpdateEvent* pEvent) {
  for (auto& callback : _errorStatusUpdateSubCallbacks) {
    callback(pEvent);
  }
}

void PubSub::subscribe(std::function<void(ErrorStatusUpdateEvent*)> callback) {
  _errorStatusUpdateSubCallbacks.push_back(callback);
}

}  // namespace pubsub_utils
}  // namespace bigpjtemplate
