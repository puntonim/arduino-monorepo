/*
 * LED DEMO
 * ========
 * Blink a led.
 *
 * Docs: https://www.youtube.com/watch?v=G8S1j8UQfS0
 *
 * Circuit:
 *  LED - (shorter leg) > Arduino GND
 *  LED + (longer leg) > resistor 270 Ohm > Arduino pin 5 
 */

int pinLED = 5;

void setup() {
  pinMode(pinLED, OUTPUT);
}

void loop() {
  // Docs: https://docs.arduino.cc/language-reference/en/functions/digital-io/digitalwrite/
  digitalWrite(pinLED, HIGH);
  delay(100);
  digitalWrite(pinLED, LOW);
  delay(100);
}
