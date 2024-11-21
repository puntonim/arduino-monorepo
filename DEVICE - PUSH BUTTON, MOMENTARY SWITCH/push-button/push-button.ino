/*
 * PUSH BUTTON
 * ===========
 * Detect the push of a button and turn ON the builtin LED.
 *
 * Docs:
 *  - https://docs.arduino.cc/built-in-examples/basics/DigitalReadSerial/
 *  - https://docs.arduino.cc/built-in-examples/digital/Button/
 */

const int pushButtonPin = 2;
const int integratedLedPin = 13;

void setup() {
  Serial.begin(57600);
  pinMode(pushButtonPin, INPUT);
  // Declare the integrated LED as output;
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  bool isButtonPressed = digitalRead(pushButtonPin);
  Serial.println(isButtonPressed);
  // Turn ON the LED;
  digitalWrite(LED_BUILTIN, isButtonPressed);
  delay(100);
}
