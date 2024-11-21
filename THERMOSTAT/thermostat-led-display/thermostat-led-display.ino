/* THERMOSTAT, LED, DISPLAY
 * ========================
 * Display:
 *   1 row: ON|OFF desired-temp (starts at 18 degrees).
 *   2 row: temp e humidity from sensors.
 *   The display switches off automatically after 10 seconds.
 *   The data shown on the display is updated every 1 sec.
 * Button: switch on the display.
 *   After pressing the button 1 non-repeating task is scheduled with TaskManagerIO and lives forever.
 * LED yellow: blinks when heating is ON, so when sensor T < desired T.
 *
 * Devices:
 *  1xSHT-85: thermometer and hygrometer sensor
 *  1xDS18B20: thermometer sensor
 *  1x push button
 *  1x display I2C 16x2 
 *  1x yellow LED
 *
 * Libs:
 *  - arduino-sht by Johannes Winkelmann, Andreas Brauchli https://github.com/Sensirion/arduino-sht
 *  - DallasTemperature by Miles Burton https://docs.arduino.cc/libraries/dallastemperature/
 *  - LiquidCrystalIO by Arduino, Adafruit, theCodersCorner https://docs.arduino.cc/libraries/liquidcrystal/
 *  - TaskManagerIO by davetcc https://github.com/TcMenu/TaskManagerIO
 */

// Set IS_DEBUG to false on PRODUCTION.
#define IS_DEBUG true

#include <list>

#include <SHTSensor.h>
#include <DallasTemperature.h>
#include <LiquidCrystalIO.h>
#include <IoAbstractionWire.h>
#include <TaskManagerIO.h>

#include "common-exceptions.h"
#include "errors.h"
#include "display-device.h"
#include "sht85-sensor.h"
#include "ds18b20-sensor.h"
#include "led-device.h"
#include "button-device.h"
#include "domain.h"

void setup() {
#if IS_DEBUG == true
  Serial.begin(57600);
  Serial.println("START");
#endif

  sht85Sensor.setup();
  ds18b20Sensor.setup();
  buttonDevice.setup();
  errorLedDevice.setup();
  heatingLedDevice.setup();
  domain.setup();
  displayDevice.setup();
}

void loop() {
  // To debug TaskManagerIO's tasks.
  // printDebugTasks();

  taskManager.runLoop();
}

/*
TODO
 - Github!!

 - move the wiring settings to the main file
   or to a header file.

 - Refactor:
    - the button is told by the domain what to execute on press (devices are agnostic of domain logic and other devices)
    - the display is told by the domain if heating is ON/OFF
    - the display is told by the domain what are the sensor readings at this second
      The domain.run() runs every 2 mins,
       but when the button is pressed (and the display is On) then it runs every 1 second, but only
       if it's not running already.
       This is usefull when reading the T from a remote sensor.
       Add a white LED which is on when domain.run() runs.
    - The heatingLed should blink independently at 1 sec rate when heating is ON.
      In the future this might be connected to the actual relay and thus be ON/OFF without blinking.

 - add time counter that runs in domain.run()
 - add PID algo: https://playground.arduino.cc/Code/PIDLibrary/
 - add rotary encoder for temp (or hold a button)
 - add rotary encoder for time (or hold a button)

 - Optimization: cache sensor data for 1 sec.
   This could be done in each sensor's getData() method.
   However this makes sense only if we are reading data faster than 1 sec (fi. if we have 2
    concurrent tasks that reads sensors data at 1 sec rate each).
*/
