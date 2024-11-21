/*
 * PUSH BUTTON TOGGLE WITH DEBOUNCING
 * ==================================
 * Detect the push of a button and toggle the state of the bultin LED
 *  but with debouncing.
 * Note: push buttons are momentary switches.
 *
 * The debouncing is meant to solve the issue due to the fact that
 *  the transition of voltage from LOW to HIGH (or the oppsite) when
 *  pushing a button is not instantaneous and the reading can happen
 *  during this transition. This introduces noise. Eg. the button was
 *  release but the next reading is still HIGH because of the time taken
 *  to transition.
 * It is actually not really a big issue, it can be totally ignored.
 * It is not an issue at all if we keep track of the previous state
 *  and we consider the button triggered when it goes HIGH > LOW. 
 * To know more about this issue:
 *  https://projecthub.arduino.cc/ronbentley1/understanding-and-using-button-switches-4fa481
 *
 * PROS
 * ----
 * - It avoids the noise introduced by the physical transition of the buttons by
 *    using debouncing.
 * CONS
 * ----
 * - The debouncing is unnecessary if we keep track of the previous state
 *    and we consider the button triggered when it goes HIGH > LOW. 
 *
 * Docs: https://projecthub.arduino.cc/ronbentley1/understanding-and-using-button-switches-4fa481
 */

const int pushButtonPin = 2;

bool isLedOn = false;
bool isTogglePending = false;
bool lastIsButtonPressed = false;

unsigned long tooglePendingTs;
// The debounce time; increase if the output flickers.
// Set this to 500 for testing to maximize the debounce effect.
const unsigned debounceDelay = 50;

void setup() {
  // Serial.begin(57600);
  pinMode(pushButtonPin, INPUT);
  // Declare the builtin LED as output;
  pinMode(LED_BUILTIN, OUTPUT);
}

bool isButtonTriggered(bool doDebounce = false) {
  bool isPressed = digitalRead(pushButtonPin);
  // Serial.println(isPressed);
  // We will cahce the value of millis() to avoid calling it twice, as it is expensive.
  unsigned long now = 0;

  if (!doDebounce) {
    // If we dont need to debounce then just toggle the LED
    //  only after releasing the button so during the transition ON > OFF.
    //  Otherwise holding the button would cause flickering.
    if (!isPressed && lastIsButtonPressed) {
      return true;
    }
  }

  // If we need to debounce then we need to use a timer.
  if (isPressed) {
    isTogglePending = true;
    now = millis();
    tooglePendingTs = now;
    return false;
  }

  if (isTogglePending && !isPressed) {
    if (now == 0) {
      now = millis();
    }
    if (now - tooglePendingTs > debounceDelay) {
      isTogglePending = false;
      return true;
    }
  }

  return false;
}

void loop() {
  const bool DO_DEBOUNCE = true;
  if (isButtonTriggered(DO_DEBOUNCE)) {
    isLedOn = !isLedOn;
    digitalWrite(LED_BUILTIN, isLedOn);
  }
  // A delay of 90-100 is ok if the button is not meant to be pressed fastly.
  // Otherwise a lower value, like 50.
  delay(90);
}
