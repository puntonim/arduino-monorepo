#ifndef _SETTINGS_H
#define _SETTINGS_H

#include <DallasTemperature.h>

#include "utils/time-utils.h"

// Set IS_DEBUG to false on PRODUCTION.
#define IS_DEBUG true
// Set DO_ENABLE_FILLING_BLANKS to true to fill each row of the display with
//  blank spaces to overwrite old text, longer than the one we are displaying.
//  Howerver this can hide some bugs (concurrent prints) so we might disable it
//  on dev by setting DO_ENABLE_FILLING_BLANKS = false.
#define DO_ENABLE_FILLING_BLANKS true

namespace tstat {
namespace settings {

// Under this T, the heating wis turned ON.
const unsigned short DEFAULT_TARGET_T = 18;
const time_utils::Time DEFAULT_TIMER = {0, 1, 7};  // {H, H, S}.

// Seconds after which the display switches off.
const unsigned short DISPLAY_SWITCHOFF_TIMER = 20;
const unsigned short HEATING_DOMAIN_RUN_PERIOD = 5000;  // msec.

// Buttons.
const unsigned short DISPLAY_BUTTON_PIN = 2;
const unsigned short TARGET_T_BUTTON_PIN = 3;
const unsigned short TIMER_BUTTON_PIN = 4;

// DS18B20 sensor.
const unsigned short DS18B20_SENSOR_PIN = 8;
const DeviceAddress DS18B20_SENSOR_ADDRESS = {40, 255, 100, 31,
                                              94, 120, 150, 103};
const unsigned short DS18B20_DATA_CACHE_PERIOD = 1000;  // msec.

// DS18B20 sensor.
const unsigned short SHT85_DATA_CACHE_PERIOD = 1000;  // msec.

// Leds.
const unsigned short HEATING_LED_PIN = 11;
const unsigned short HEATING_LED_BRIGHTNESS_VALUE = 30;
const unsigned short HEATING_LED_BLINKING_PERIOD = 1000;  // msec.
const unsigned short ERROR_LED_PIN = 10;
const unsigned short ERROR_LED_BRIGHTNESS_VALUE = 100;
const unsigned short ERROR_LED_BLINKING_PERIOD = 100;  // msec.
const unsigned short DOMAIN_LED_PIN = 9;
const unsigned short DOMAIN_LED_BRIGHTNESS_VALUE = 20;

}  // namespace settings
}  // namespace tstat
#endif
