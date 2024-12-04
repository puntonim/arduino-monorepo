/*
 * PUSH BUTTON TOGGLE WITH INTERRUPTS
 * ==================================
 * TLDR: do not use interrupts without debouncing. The main goal of using interrupts
 *  is to avoid polling, so doing less waste work. But interrupts are duped
 *  for the same event, due to the fact that the physical transition of the button
 *  from HIGH to LOW (and viceversa) takes time and it produces dupes interrupts.
 *  So you need debouncing.
 *
 * Note: push buttons are momentary switches.
 * Note: with a pull-down resistor which is not the smartest choice.
 *
 * PROS
 * ----
 * - None
 * CONS
 * ----
 * - It does not work due to the duped interrupts.
 *
 * Docs:
 *  - https://docs.arduino.cc/built-in-examples/basics/DigitalReadSerial/
 *  - https://docs.arduino.cc/built-in-examples/digital/Button/
 */

const int pushButtonPin = 2;

volatile bool isButtonPressed = false;
bool isLedOn = false;
bool isSetupComplete = false;

void setup() {
  Serial.begin(57600);
  pinMode(pushButtonPin, INPUT);
  // Declare the integrated LED as output;
  pinMode(LED_BUILTIN, OUTPUT);

  // Docs: https://docs.arduino.cc/language-reference/en/functions/external-interrupts/attachInterrupt/
  attachInterrupt(digitalPinToInterrupt(pushButtonPin),
                  interruptHandler,
                  FALLING); // FALLING = when released.
  isSetupComplete = true;
}

void interruptHandler() {
  if (!isSetupComplete) {
    return;
  }
  isButtonPressed = true;
}

void loop() {
  if (isButtonPressed) {
    isLedOn = !isLedOn;
    digitalWrite(LED_BUILTIN, isLedOn);
    isButtonPressed = false;
  }

  // A delay of 90-100 is ok if the button is not meant to be pressed fastly.
  // Otherwise a lower value, like 50.
  delay(90);
}
