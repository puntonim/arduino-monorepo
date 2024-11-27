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

#include <list>
#include <functional>
#include <memory>

#include <SHTSensor.h>
#include <DallasTemperature.h>
#include <LiquidCrystalIO.h>
#include <IoAbstractionWire.h>
#include <TaskManagerIO.h>

#include "list-utils.h"
#include "time-utils.h"
#include "pubsub-utils.h"
#include "settings.h"
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
  domainLedDevice.setup();
  displayDevice.setup();
  domain.setup();
}

void loop() {
  // To debug TaskManagerIO's tasks.
  // printDebugTasks();

  taskManager.runLoop();
}

/*
TODO
 - Domain::_checkTime()
 - convert SHT85 from reference to return struct
 - convert many int vars to short
 - use namespace for all utils: https://stackoverflow.com/a/46499087/1969672
 - when the timer is over then stop the domain task
   It should start again when I set a new timer

 - new HW:
    - rotary encoders (temp, time)
    - bluetooth to another board
    - switch for heating pump

 - Use PID algo in domain: https://playground.arduino.cc/Code/PIDLibrary/

 - Cloud and web:
    - send data to IoT cloud or custom BE to track:
      - T and H over time
      - heating ON/OFF 
      These data could collected and sent to the remote every 5 mins
      Or they could be collected every 1 min, stored in a SD card and sent to the remote every 5 mins 
    - Hmmm: receive data from remote (IoT cloud? regular HTTP reqs?)
      But I don't rellay need the feature to switch ON/OFF the heating from remote
*/
