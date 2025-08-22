#include "utils/error-utils.h"

#include "settings.h"
#include "utils/pubsub-utils.h"
#include "utils/remote-log-utils.h"

namespace myproject {
namespace error_utils {

// "Soft" *singleton* global object defined here,
//  but defined as extern and initialized in error-utils.h.
ErrorMgr errorMgr;

/**
 * Print the latest error to Serial and post it to iot-be /iot/remote-logs/
 * endpoint.
 */
void ErrorMgr::_handleErrors(bool doSkipRemoteLog /* = false */) {
  // Get the latest error.
  Error e = _errors.back();

  if (!doSkipRemoteLog) {
    // clang-format off
    // Allocate a char[] that is long enough (longer than needed is ok).
    char msg[strlen(e.errorId)+strlen(e.msgForSerial)+strlen(e.msgForDisplay)+80];
    sprintf(
      msg,
      // Double-escaping quotes because this message is text wrapped inside JSON,
      //  so quotes are not allowed.
      "{"
        "\\\"errorId\\\": \\\"%s\\\","
        "\\\"msgForSerial\\\": \\\"%s\\\","
        "\\\"msgForDisplay\\\": \\\"%s\\\""
      "}",
      e.errorId,
      e.msgForSerial,
      e.msgForDisplay ? e.msgForDisplay : ""
    );
    // clang-format on
    remote_log_utils::remoteLog.postRemoteLog(msg, true);
  }

#if IS_DEBUG == true
  Serial.println((String) "Error: " + e.msgForSerial);
#endif
}

void ErrorMgr::addError(const char* errorId, const char* msgForSerial,
                        const char* msgForDisplay /* = nullptr */,
                        bool doSkipRemoteLog /* = false */) {
  // Add the error only if there is no an existenting error with the
  //  same id and the error list if less than MAX_ERRORS_LIST_SIZE items.
  if (isThereErrorId(errorId) ||
      _errors.size() >= settings::MAX_ERRORS_LIST_SIZE) {
    return;
  }

  _errors.push_back(Error(errorId, msgForSerial, msgForDisplay));

  // Publish the event so that the errorLed can start blinking, and
  //  the LCD display (if any) to show the message.
  pubsub_utils::pubSub.publish(new pubsub_utils::ErrorStatusUpdateEvent(true));
  _handleErrors(doSkipRemoteLog);
}

/**
 * Remove an error.
 *
 * Errors can be removed, as the error situation is now resolved.
 * For example: a loop reads a sensor every 1 sec. At a certain moment the
 *  sensor returns an error, but in the next loop the sensor returns the data
 *  (so there error situation is now resolved).
 */
void ErrorMgr::removeError(const char* errorId) {
  _errors.remove_if([errorId](Error e) { return e.errorId == errorId; });

  if (!areThereErrors()) {
    pubsub_utils::pubSub.publish(
        new pubsub_utils::ErrorStatusUpdateEvent(false));
  }
}

void ErrorMgr::removeAllErrors() {
  _errors.clear();

  pubsub_utils::pubSub.publish(new pubsub_utils::ErrorStatusUpdateEvent(false));
}

bool ErrorMgr::areThereErrors() { return _errors.size() > 0; }

std::list<Error> ErrorMgr::getErrors() { return _errors; }

/**
 * Check if there is the error with the given errorId.
 */
bool ErrorMgr::isThereErrorId(const char* errorId) {
  for (Error e : _errors) {
    if (e.errorId == errorId) return true;
  }
  return false;
}

void ErrorMgr::_printErrors() {
#if IS_DEBUG == true
  Serial.println("\nErrors: ");

  for (Error e : _errors) {
    Serial.println((String)e.errorId + " | " + e.msgForSerial + " | " +
                   e.msgForDisplay);
  }
#endif
}

}  // namespace error_utils
}  // namespace myproject