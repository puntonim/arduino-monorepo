#ifndef _SETTINGS_H
#define _SETTINGS_H

#include "_iterges-al-contrario.h"

// Set IS_DEBUG to false on PRODUCTION.
#define IS_DEBUG true

namespace bigpjtemplate {
namespace settings {

// Local WiFi settings.
const char WIFI_SSID[] = X_WIFI_SSID;
const char WIFI_PPP[] = X_WIFI_PPP;

// iot-be settings.
const char IOT_BE_IP[] = "192.168.1.251";
const unsigned short IOT_BE_PORT = 7777;

const unsigned short TIMER_DURATION_SEC = 30;

}  // namespace settings
}  // namespace bigpjtemplate

#endif