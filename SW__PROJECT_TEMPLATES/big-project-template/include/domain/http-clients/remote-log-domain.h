#ifndef _REMOTE_LOG_DOMAIN_H
#define _REMOTE_LOG_DOMAIN_H

#include <ArduinoHttpClient.h>

#include "devices/wifi-device.h"
#include "settings.h"

namespace bigpjtemplate {

class RemoteLogDomain {
 private:
  // This should be done only once, in case of many requests.
  HttpClient _client =
      HttpClient(wifiClient, settings::IOT_BE_IP, settings::IOT_BE_PORT);

 public:
  void setup();
  void postRemoteLog(const char* message);
};

// "Soft" *singleton* global object defined as extern and initialized here,
//  but also defined in remote-log-domain.cpp. `extern` just means that the var
//  is defined elsewhere. And we need it here because we want it initialized on
//  on boot.
extern RemoteLogDomain remoteLogDomain;

}  // namespace bigpjtemplate

#endif
