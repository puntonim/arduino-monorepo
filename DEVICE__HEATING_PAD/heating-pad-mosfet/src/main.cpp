/* HEATING PAD, MOSFET
 * ===================
 * The heating pad, powered by an external PSU, is controlled by Arduino
 *  using a MOSFET that sets how much current to drain to the pad (PWM).
 *
 * Devices: ??????????????????????????
 *  1xSHT-85: thermometer and hygrometer sensor
 *  1xDS18B20: thermometer sensor
 *  3x push button
 *  1x display I2C 16x2
 *  3x LEDs
 *
 * Libs: none.
 */

// TODO .................

#include <Arduino.h>

const uint8_t MOSFET_PIN = 3;

void setup() {
  Serial.begin(57600);
  Serial.println("START");

  pinMode(MOSFET_PIN, OUTPUT);
  analogWrite(MOSFET_PIN, 0);  // Range: 0-255.
}

void loop() {
  Serial.println("Power: 5/255");
  delay(5000);
}
