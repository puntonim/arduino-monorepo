/*
 * LED PWM FADE
 * ============
 * Fade a led using PWM.
 *
 * Docs: https://www.youtube.com/watch?v=ZcFU_K7ilA8
 *
 * Circuit:
 *  LED - > Arduino GND
 *  LED + > resistor 270 Ohm > Arduino pin 5 (PWM)
 */

int pinLED = 5;
int brightness = 0;
int fadeAmount = 5;

void setup() {
  pinMode(pinLED, OUTPUT);

  // Dim LED setting it to 1.
  // Docs: https://www.arduino.cc/reference/it/language/functions/analog-io/analogwrite/
  analogWrite(pinLED, 1); // 0-255 values.
  delay(5000);
}

void loop() {
  // Fade the LED.

  analogWrite(pinLED, brightness); // 0-255 values.
  // When the LED is off, keep it off for 1 sec.
  if (brightness == 0) {
    delay(1000);
  }

  brightness += fadeAmount;
  if (brightness <= 0 | brightness >= 100) {
    // Brightness can go from 0 to 255, but I am stopping at 100.
    fadeAmount *= -1;
  }
  delay(90);
}

