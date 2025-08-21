#ifndef _ERROR_UTILS_H
#define _ERROR_UTILS_H

#include <list>

#define SENSOR_ERROR -127

namespace bigpjtemplate {
namespace error_utils {

class Error {
 public:
  // Unique identifier for the error: eg. WIFI_CONNECT_AP_ERROR.
  const char* errorId;
  // Text to be used for Serial.println();
  const char* msgForSerial;
  // Text to be printed on a LCD display, optional;
  const char* msgForDisplay;

  Error(const char* errorId, const char* msgForSerial,
        const char* msgForDisplay = nullptr)
      : errorId(errorId),
        msgForSerial(msgForSerial),
        msgForDisplay(msgForDisplay) {}
};

class ErrorMgr {
 private:
  // Required operations on the list:
  //  - sorted by insertion;
  //  - add and extract like a queue;
  //  - remove element by value  in any position.
  // So using a `list` (doubly-linked list) and not a `forward_list`
  //  (singly-linked list) because it needs to operate like a queue (with
  //  `push_back()`). And not using a `queue` because we need to be able to
  //  `remove` elements at any position.
  std::list<Error> _errors;
  void _handleErrors(bool doSkipRemoteLog = false);
  void _printErrors();

 public:
  void addError(const char* errorId, const char* msgForSerial,
                const char* msgForDisplay = nullptr,
                bool doSkipRemoteLog = false);
  void removeError(const char* errorId);
  void removeAllErrors();
  bool areThereErrors();
  bool isThereErrorId(const char* errorId);
  std::list<Error> getErrors();
};

// "Soft" *singleton* global object defined as extern and initialized here,
//  but also defined in error-utils.cpp. `extern` just means that the var is
//  defined elsewhere. And we need it here because we want it initialized on
//  on boot.
extern ErrorMgr errorMgr;

}  // namespace error_utils
}  // namespace bigpjtemplate

#endif