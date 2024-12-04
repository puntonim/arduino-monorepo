/*
 * PUSH BUTTON
 * ===========
 * Detect the push of a button and turn ON the builtin LED while the button is pressed.
 *
 * Note: the button is connected with a pull-up resistor, which is the smartest
 *  choice as Arduino has integrated pull-up resistors on every PIN (so you don't 
 *  even need to add an extra resistor). HIGH-LOW state are inverted compared to
 *  the pull-down resistor.
 *
 * Docs:
 *  - https://docs.arduino.cc/built-in-examples/basics/DigitalReadSerial/
 *  - https://docs.arduino.cc/built-in-examples/digital/Button/
 */

const int BUTTON_PIN = 2;

void setup() {
  Serial.begin(57600);
  Serial.println("START");

  // Options: INPUT, OUTPUT, or INPUT_PULLUP.
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  // Declare the integrated LED on pin 13 as output;
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  bool isButtonPressed = !digitalRead(BUTTON_PIN);
  Serial.println(isButtonPressed == HIGH ? "HIGH" : "LOW");
  // Turn ON the LED.
  digitalWrite(LED_BUILTIN, isButtonPressed);
  delay(100);
}
