/**
 * OOP, HEADERS, SINGLETON, NAMESPACE
 * ==================================
 * This example puts together a few OOP things:
 *  - OOP, so classes and instances
 *  - header file, so *.h
 *  - namespace, to group vars
 *  - singleton, the design pattern.
 *
 * Libs:
 *  - TaskManagerIO by davetcc
 *    https://github.com/TcMenu/TaskManagerIO
 */

#include <Arduino.h>

#include "led-device.h"

using namespace oopexample;

void setup() {
  Serial.begin(57600);
  unsigned short i = 30;
  while (!Serial && i--) delay(1);
  Serial.println("START " __FILE__ " compiled on " __DATE__);

  errorLedDevice.setup();

  errorLedDevice.startBlinking();
}

void loop() { taskManager.runLoop(); }