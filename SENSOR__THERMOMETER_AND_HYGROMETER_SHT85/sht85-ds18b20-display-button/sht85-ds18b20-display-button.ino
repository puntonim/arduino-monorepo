/* 1xSHT85 1xDS18B20 WITH DISPLAY AND BUTTON
 * =========================================
 * When pushing the button, show the temperatures and humidity on the display.
 *
 * Display:
 *   1 row: 18.54°C
 *   2 row: 18.23°C 30.51%
 *   The display switches off automatically after 10 seconds.
 *   The data shown on the display is updated every 1 sec.
 * Button: switch on the display.
 *   After pressing the button 1 non-repeating task is scheduled with TaskManagerIO and lives forever.
 *
 * Devices:
 *  1xSHT-85: thermometer and hygrometer sensor
 *  1xDS18B20: thermometer sensor
 *  1x push button
 *  1x display I2C 16x2 
 *
 * Libs:
 *  - arduino-sht by Johannes Winkelmann, Andreas Brauchli https://github.com/Sensirion/arduino-sht
 *  - DallasTemperature by Miles Burton https://docs.arduino.cc/libraries/dallastemperature/
 *  - LiquidCrystalIO by Arduino, Adafruit, theCodersCorner https://docs.arduino.cc/libraries/liquidcrystal/
 *  - TaskManagerIO by davetcc https://github.com/TcMenu/TaskManagerIO
 */

#include <SHTSensor.h>
#include <DallasTemperature.h>
#include <LiquidCrystalIO.h>
#include <IoAbstractionWire.h>
#include <TaskManagerIO.h>

#define SENSOR_ERROR -127

// Declaration of functions with default args.
void switchOnDisplay(bool doCancelExistingSwitchOffDisplayTask = true);
void switchOffDisplay(bool doResetSwitchOffDisplayTaskId = true);

// Init all the known TaskManagerIO' tasks ids to TASKMGR_INVALIDID (which is the int 65535).
// And doing it here, instead of the appropriate file, so we can use them in printDebugTasks().
taskid_t switchOffDisplayTaskId = TASKMGR_INVALIDID;
taskid_t displayDataTaskId = TASKMGR_INVALIDID;

void setup() {
  Serial.begin(57600);

  setupSht85();
  setupDs18b20();
  setupButton();
  setupDisplay();
}

void loop() {
  // TODO disable this debug.
  // printDebugTasks();

  taskManager.runLoop();
}
