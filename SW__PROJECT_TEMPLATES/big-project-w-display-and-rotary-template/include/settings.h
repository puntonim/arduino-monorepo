#ifndef _SETTINGS_H
#define _SETTINGS_H

#include <Arduino.h>  // For uint8_t.

#include "_iterges-al-contrario.h"
#include "utils/time-utils.h"

// Set IS_DEBUG to false on PRODUCTION.
#define IS_DEBUG true
// Set DO_ENABLE_FILLING_BLANKS to true to fill each row of the display with
//  blank spaces to overwrite old text, longer than the one we are displaying.
//  Howerver this can hide some bugs (concurrent prints) so we might disable it
//  on dev by setting DO_ENABLE_FILLING_BLANKS = false.
#define DO_ENABLE_FILLING_BLANKS true

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
const char IOT_BE_REMOTE_LOG_APP_ID[] =
    "BIG_PROJECT_TEMPLATE_W_DISPLAY_AND_ROTARY";
const char IOT_BE_REMOTE_LOG_DEVICE_ID[] = "DEV_ARDU";

// Domain.
const unsigned short MAIN_DOMAIN_RUN_PERIOD = 5;  // sec.

// Errors.
const unsigned short MAX_ERRORS_LIST_SIZE = 10;

// Display.
// Seconds after which the display switches off.
const unsigned short DISPLAY_SWITCHOFF_TIMER = 10;  // 10 sec.

// Rotary encoders.
// When rotating the timer rotary encoder, then this time is added/subtracted.
const time_utils::Time DELTA_TIME_ON_ROTARY_ROTATION = {0, 0, 1};  // {H, M, S}.
const uint8_t TIMER_ROTARY_CLK_PIN = 5;
const uint8_t TIMER_ROTARY_DT_PIN = 6;
const uint8_t TIMER_ROTARY_SW_PIN = 7;

// Timer.
// When starting a timer, then this is the initial value.
const time_utils::Time INITIAL_TIMER = {0, 0, 30};  // {H, M, S}.

}  // namespace settings
}  // namespace bigpjtemplate

#endif