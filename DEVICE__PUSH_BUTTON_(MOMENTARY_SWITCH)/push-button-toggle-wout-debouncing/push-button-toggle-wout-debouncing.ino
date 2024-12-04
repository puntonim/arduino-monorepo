/*
 * PUSH BUTTON TOGGLE WITHOUT DEBOUNCING
 * =====================================
 * Detect the push of a button and toggle the state of the bultin LED
 *  but without debouncing.
 *
 * Note: push buttons are momentary switches.
 * Note: with a pull-down resistor which is not the smartest choice.
 *
 * The debouncing is mean to solve the issue due to the fact that
 *  the transition of voltage from LOW to HIGH (or the oppsite) when
 *  pushing a button is not instantaneous and the reading can happen
 *  during this transition. This introduces noise.
 * It is actually not really a big issue, it can be totally ignored.
 * It is not an issue at all if we keep track of the previous state
 *  and we consider the button triggered when it goes HIGH > LOW. 
 * To know more about this issue:
 *  https://projecthub.arduino.cc/ronbentley1/understanding-and-using-button-switches-4fa481
 *
 * PROS
 * ----
 * - Simple. The best choice if you don't want to use interrupts, nor a lib.
 * - It doesn't use debouncing, but debouncing is unnecessary if we keep
 *    track of the previous state and we consider the button triggered when
 *    it goes HIGH > LOW.
 * CONS
 * ----
 * - Not as effcient as the version with interrupts and debouncing, maybe with a lib.
 *
 * Docs:
 *  - https://docs.arduino.cc/built-in-examples/basics/DigitalReadSerial/
 *  - https://docs.arduino.cc/built-in-examples/digital/Button/
 */

const int pushButtonPin = 2;

bool lastIsButtonPressed = false;
bool isLedOn = false;

void setup() {
  Serial.begin(57600);
  pinMode(pushButtonPin, INPUT);
  // Declare the integrated LED as output;
  pinMode(LED_BUILTIN, OUTPUT);
}

bool isButtonTriggered() {
  bool isButtonPressed = digitalRead(pushButtonPin);
  Serial.println(isButtonPressed);

  // Toggle the LED only after releasing the button so
  //  during the transition ON > OFF. Otherwise holding the
  //  button would cause flickering.
  if (!isButtonPressed && lastIsButtonPressed) {
    lastIsButtonPressed = isButtonPressed;
    return true;
  }

  lastIsButtonPressed = isButtonPressed;
  return false;
}

void loop() {
  if (isButtonTriggered()) {
    isLedOn = !isLedOn;
    digitalWrite(LED_BUILTIN, isLedOn);
  }

  // A delay of 90-100 is ok if the button is not meant to be pressed fastly.
  // Otherwise a lower value, like 50.  
  delay(90);
}
