/* DS18B20 WITH DISPLAY AND BUTTON
 * ===============================
 * When pushing the button, show the T on the display.
 * The display switches off automatically after 10 seconds.
 * The data shown on the display is updated every second.
 *
 * Display:
 *   1 row: 16.3°C
 *   2 row: [blank]
 *   The display switches off automatically after 10 seconds.
 *   The data shown on the display is updated every 1 sec.
 * Button: switch on the display.
 *   After pressing the button 1 non-repeating task is scheduled with TaskManagerIO and lives forever.
 *
 * Devices:
 *  1xDS18B20: thermometer sensor
 *  1x push button
 *  1x display I2C 16x2 
 * 
 * Libs: 
 *  - DallasTemperature by Miles Burton https://docs.arduino.cc/libraries/dallastemperature/
 *  - LiquidCrystalIO by Arduino, Adafruit, theCodersCorner https://docs.arduino.cc/libraries/liquidcrystal/
 *  - TaskManagerIO by davetcc https://github.com/TcMenu/TaskManagerIO
 */

#include <DallasTemperature.h>
#include <LiquidCrystalIO.h>
#include <IoAbstractionWire.h>

#define SENSOR_ERROR -127

// Declaration of functions with default args.
void switchOnDisplay(bool doCancelExistingSwitchOffDisplayTask = true);
void switchOffDisplay(bool doResetSwitchOffDisplayTaskId = true);

void setup() {
  Serial.begin(57600);

  setupDs18b20();
  setupButton();
  setupDisplay();
}

void loop() {
  // TODO disable this debug.
  // printDebugTasks();

  taskManager.runLoop();
}
