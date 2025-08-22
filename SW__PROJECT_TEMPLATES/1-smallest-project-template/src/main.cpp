/**
 * 1. SMALLEST PROJECT TEMPLATE
 * ============================
 * Just the shortest code to print to Serial Monitor.
 *
 * Libs: none
 */

#include <Arduino.h>

void setup() {
  Serial.begin(57600);
  unsigned short i = 30;
  while (!Serial && i--) delay(1);

  Serial.println("START " __FILE__ " compiled on " __DATE__);
}

void loop() {
  Serial.print("Loop: ");
  Serial.println(millis());

  delay(1000);  // msec.
}
