/**
 * 3. BIG PROJECT TEMPLATE WITH DISPLAY AND ROTARY
 * ===============================================
 * Template for a big projects. It includes:
 *  - WiFi with remote logging
 *  - error and domain leds
 *  - error collection to autom print to Serial and send to iot-be via HTTP
 *  - task manager
 *  - pub/sub
 *  - settings and secrets
 *  - modules, namespace, oop and others.
 *
 * What it does:
 *  on boot it connects to WiFi and logs to iot-be,
 *  then after 20 secs from boot it pretends there is a test error (so the error
 *   led starts blinking and the error is logged to Serial and to iot-be via
 *   HTTP) that goes aways after 5 secs,
 *  and after 30 secs from boot it switches on the LED matrix for 5 secs.
 *
 * Sw architecture:
 *   after the initial setup of the WiFi,
 *   a task running `MainDomain::run` is scheduled every 5 secs.
 *   When the test error is added with `error_utils::errorMgr.addError` then it
 *    is automatically logged to Serial and to iot-be via HTTP and the event
 *    ERROR_STATUS_UPDATE_EVENT is published, which causes the subscribed
 *    errorLed to start blinking (until the error is removed).
 *
 * Libs:
 *  - TaskManagerIO by davetcc
 *    https://github.com/TcMenu/TaskManagerIO
 *  - ArduinoHttpClient by Arduino
 *    https://github.com/arduino-libraries/ArduinoHttpClient
 *  - LiquidCrystalIO by Arduino, Adafruit, theCodersCorner
 *    https://docs.arduino.cc/libraries/liquidcrystalio/
 *  - IoAbstraction by tcmenu (required when using LiquidCrystalIO I2C version
 *     and required by the rotary encoders)
 *    https://docs.arduino.cc/libraries/ioabstraction/
 */

// TODO BUG !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// See utils/remote-log-utils.cpp.

#include <Arduino.h>

#include "devices/display-device.h"
#include "devices/led-device.h"
#include "devices/rotary-encoder-device.h"
#include "devices/wifi-device.h"
#include "domain/main-domain.h"
#include "utils/error-utils.h"
#include "utils/remote-log-utils.h"

using namespace myproject;

void setup() {
#if IS_DEBUG == true
  Serial.begin(57600);
  unsigned short i = 30;
  while (!Serial && i--) delay(1);
  Serial.println("START " __FILE__ " compiled on " __DATE__);
#endif

  displayDevice.setup();
  errorLedDevice.setup();
  domainLedDevice.setup();
  ledMatrixDevice.setup();
  rotaryEncoderDevices.setup();

  // The WiFi setup is slow, so it should happen after the display and
  //  buttons, but before anything that uses error-utils or remote-log-utils
  //  because those make HTTP requests. But mind that loop() starts running
  //  only after this whole setup, including wifiDevice, is complete.
  wifiDevice.setup();

  remote_log_utils::remoteLog.postRemoteLog(
      "Just booted and connected to WiFi");

  // Finally start the main domain.
  mainDomain.setup();
}

void loop() {
  // To debug TaskManagerIO's tasks.
  // myproject::task_manager_utils::printDebugTasks(true);

  taskManager.runLoop();
}
