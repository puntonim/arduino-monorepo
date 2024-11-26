// Set IS_DEBUG to false on PRODUCTION.
#define IS_DEBUG true
// Set DO_ENABLE_FILLING_BLANKS to true to fill each row of the display with blank
//  spaces to overwrite old text, longer than the one we are displaying.
//  Howerver this can hide some bugs (concurrent prints) so we might disable it
//  on dev by setting DO_ENABLE_FILLING_BLANKS = false.
#define DO_ENABLE_FILLING_BLANKS true


struct Settings {
  // Under this T, the heating wis turned ON.
  const unsigned int TARGET_T = 18;
  // Seconds after which the display switches off.
  const unsigned int DISPLAY_SWITCHOFF_TIMER = 20;
  const unsigned int DOMAIN_RUN_PERIOD = 1000;  // msec.

  // Button.
  const int PUSH_BUTTON_PIN = 2;

  // DS18B20 sensor.
  const unsigned int DS18B20_SENSOR_PIN = 4;
  const DeviceAddress DS18B20_SENSOR_ADDRESS = { 40, 255, 100, 31, 94, 120, 150, 103 };
  const unsigned int DS18B20_DATA_CACHE_PERIOD = 1000;  // msec.

  // DS18B20 sensor.
  const unsigned int SHT85_DATA_CACHE_PERIOD = 1000;  // msec.

  // Leds.
  const unsigned int HEATING_LED_PIN = 6;
  const unsigned int HEATING_LED_BRIGHTNESS_VALUE = 30;
  const unsigned int HEATING_LED_BLINKING_PERIOD = 1000;  // msec.
  const unsigned int ERROR_LED_PIN = 9;
  const unsigned int ERROR_LED_BRIGHTNESS_VALUE = 100;
  const unsigned int ERROR_LED_BLINKING_PERIOD = 100;  // msec.
  const unsigned int DOMAIN_LED_PIN = 5;
  const unsigned int DOMAIN_LED_BRIGHTNESS_VALUE = 20;

} settings;