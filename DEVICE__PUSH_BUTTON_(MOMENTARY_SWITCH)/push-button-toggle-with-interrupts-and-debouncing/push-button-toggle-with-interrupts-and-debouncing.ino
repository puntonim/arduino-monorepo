/*
 * PUSH BUTTON TOGGLE WITH INTERRUPTS AND DEBOUNCING
 * =================================================
 * TLDR: this is the most efficient way to handle buttons.
 *  However Arduino Uno has onl2 2 PINs that support interrupts.
 *
 * Note: do not use interrupts without debouncing. The main goal of using interrupts
 *  is to avoid polling and so doing less waste work. But interrupts are duped
 *  for the same event, due to the fact that the physical transition of the button
 *  from HIGH to LOW (and viceversa) takes time and it produces dupes interrupts.
 *  So you need debouncing.
 * Note: push buttons are momentary switches.
 * Note: with a pull-down resistor which is not the smartest choice.
 *
 * PROS
 * ----
 * - The most efficient way to handle buttons.
 * CONS
 * ----
 * - Arduino UNO R4 has only 2 PINs that support interrupts, see: https://docs.arduino.cc/language-reference/en/functions/external-interrupts/attachInterrupt/
 *
 * Docs:
 *  - https://projecthub.arduino.cc/ronbentley1/button-switch-using-an-external-interrupt-16d57f
 */

const int pushButtonPin = 2;

volatile bool isInterruptPending = false;
bool isLedOn = false;
bool isSetupComplete = false;
bool isTogglePending = false;
unsigned long tooglePendingTs;
// The debounce time; increase if the output flickers.
// Set this to 500 for testing to maximize the debounce effect.
const unsigned debounceDelay = 50;

void setup() {
  Serial.begin(57600);
  pinMode(pushButtonPin, INPUT);
  // Declare the integrated LED as output;
  pinMode(LED_BUILTIN, OUTPUT);

  // Docs: https://docs.arduino.cc/language-reference/en/functions/external-interrupts/attachInterrupt/
  attachInterrupt(digitalPinToInterrupt(pushButtonPin),
                  interruptHandler,
                  RISING);  // RISING = when pressed.
  isSetupComplete = true;
}

void interruptHandler() {
  if (isSetupComplete && !isInterruptPending && digitalRead(pushButtonPin)) {
    isInterruptPending = true;
  }
}

bool isButtonTriggered() {
  if (isInterruptPending) {
    bool isPressed = digitalRead(pushButtonPin);
    if (isPressed) {
      isTogglePending = true;
      tooglePendingTs = millis();
    }
    if (isTogglePending && !isPressed) {
      if (millis() - tooglePendingTs >= debounceDelay) {
        isTogglePending = false;
        isInterruptPending = false;
        return true;
      }
    }
  }
  return false;
}

void loop() {
  if (isButtonTriggered()) {
    isLedOn = !isLedOn;
    digitalWrite(LED_BUILTIN, isLedOn);
  }

  // A delay of 90-100 is ok if the button is not meant to be pressed fastly.
  // Otherwise a lower value, like 50.
  delay(100);
}