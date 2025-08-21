#ifndef _REMOTE_LOG_UTILS_H
#define _REMOTE_LOG_UTILS_H

#include <ArduinoHttpClient.h>

#include "devices/wifi-device.h"
#include "settings.h"

namespace bigpjtemplate {
namespace remote_log_utils {

class RemoteLog {
 private:
 public:
  void setup();
  void postRemoteLog(const char* message, bool isError = false);
};

// "Soft" *singleton* global object defined as extern and initialized here,
//  but also defined in remote-log-domain.cpp. `extern` just means that the var
//  is defined elsewhere. And we need it here because we want it initialized on
//  on boot.
extern RemoteLog remoteLog;

}  // namespace remote_log_utils
}  // namespace bigpjtemplate

#endif
