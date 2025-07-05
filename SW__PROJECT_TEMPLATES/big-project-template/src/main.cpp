/**
 * BIG PROJECT TEMPLATE
 * ====================
 * Template for a big projects. It includes:
 *  - WiFi with remote logging
 *  - task manager
 *  - pub/sub
 *  - settings and secrets
 *  - modules, namespace, oop and others.
 *
 * What it does:
 *  it blinks the internal LED when the 30 secs timer is over.
 *
 * Sw architecture:
 *   after the initial setup of the WiFi,
 *   the timerDomain object schedules a task once in 30 sec,
 *   the task publishes the TIMER_FINISH_EVENT,
 *   which is received by the subscribed object errorLedDevice which starts
 *    blinking.
 *
 * Libs:
 *  - TaskManagerIO by davetcc
 *    https://github.com/TcMenu/TaskManagerIO
 *  - ArduinoHttpClient by Arduino
 *    https://github.com/arduino-libraries/ArduinoHttpClient
 */

#include <Arduino.h>

#include "devices/led-device.h"
#include "devices/wifi-device.h"
#include "domain/http-clients/remote-log-domain.h"
#include "domain/timer-domain.h"

using namespace bigpjtemplate;

void setup() {
#if IS_DEBUG == true
  Serial.begin(57600);
  unsigned short i = 30;
  while (!Serial && i--) delay(1);
  Serial.println("START " __FILE__ " compiled on " __DATE__);
#endif

  wifiDevice.setup();
  errorLedDevice.setup();
  timerDomain.setup();

  remoteLogDomain.postRemoteLog("Just booted and connected to WiFi");
}

void loop() {
  // To debug TaskManagerIO's tasks.
  // tstat::task_manager_utils::printDebugTasks(true);

  taskManager.runLoop();
}