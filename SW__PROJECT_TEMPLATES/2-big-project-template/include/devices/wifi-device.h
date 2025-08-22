#ifndef _WIFI_DEVICE_H
#define _WIFI_DEVICE_H

#include <WiFiS3.h>

#include "settings.h"

namespace myproject {

class WifiDevice {
 private:
  // Note: we need to use a pointer here or the compiler will complain.
  const char* _ssid = settings::WIFI_SSID;
  const char* _ppp = settings::WIFI_PPP;
  void _printWifiConnectionInfo();

 public:
  void setup();
};

// "Soft" *singleton* global object defined as extern and initialized here,
//  but also defined in wifi-device.cpp. `extern` just means that the var is
//  defined elsewhere. And we need it here because we want it initialized on
//  on boot.
extern WifiDevice wifiDevice;
extern WiFiClient wifiClient;
// extern WiFiSSLClient wifiHttpsClient;

}  // namespace myproject

#endif