#ifndef _SETTINGS_H
#define _SETTINGS_H

#include <Arduino.h>  // For uint8_t.

#include "_iterges-al-contrario.h"

// Set IS_DEBUG to false on PRODUCTION.
#define IS_DEBUG true

namespace bigpjtemplate {
namespace settings {

// Local WiFi settings.
const char WIFI_SSID[] = X_WIFI_SSID;
const char WIFI_PPP[] = X_WIFI_PPP;

// Leds.
const uint8_t ERROR_LED_PIN = 10;
const unsigned short ERROR_LED_BRIGHTNESS_VALUE = 100;
const unsigned short ERROR_LED_BLINKING_PERIOD = 100;  // msec.
const uint8_t DOMAIN_LED_PIN = 9;
const unsigned short DOMAIN_LED_BRIGHTNESS_VALUE = 20;

// iot-be settings.
const char IOT_BE_IP[] = "192.168.1.251";
const unsigned short IOT_BE_PORT = 7777;
const char IOT_BE_REMOTE_LOG_APP_ID[] = "BIG_PROJECT_TEMPLATE";
const char IOT_BE_REMOTE_LOG_DEVICE_ID[] = "DEV_ARDU";

// Domain.
const unsigned short MAIN_DOMAIN_RUN_PERIOD = 5;  // sec.

// Errors.
const unsigned short MAX_ERRORS_LIST_SIZE = 10;

}  // namespace settings
}  // namespace bigpjtemplate

#endif