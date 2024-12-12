/*
 * PUSH BUTTON TOGGLE WITH IOABSTRACTION
 * =====================================
 * Detect the push of a button and toggle the state of the bultin LED
 *  using the lib IOAbstraction. Also, detect when the button is held down for
 *  >2 secs.
 *
 * Note: push buttons are momentary switches.
 * Note: button connected with the integrated pull-up resistor.
 * Note: Arduino UNO R4 seems to have only 2 PINs that support interrupts
 *  pin 2 and 3, see:
 *  https://docs.arduino.cc/language-reference/en/functions/external-interrupts/attachInterrupt/
 *  however this code seems to work even on pin 6 and I am sure that no tasks
 *  is scheduled to execute the polling (because I debugged it with
 *  printDebugTasks()).
 *
 * PROS
 * ----
 * - The best choice. Simple.
 * - It works with interrupts or polling.
 * - It manages debouncing and interrupts/polling automatically.
 * CONS
 * ----
 * - None.
 *
 * Devices:
 *  1x push button (using Arduino's internal pull-up resistor)
 *
 * Docs:
 *  - https://tcmenu.github.io/documentation/arduino-libraries/io-abstraction/
 *
 * Libs:
 *  - IoAbstraction by davetecc
 *    https://reference.arduino.cc/reference/en/libraries/ioabstraction/
 *  - TaskManagerIO by davetecc
 *    https://reference.arduino.cc/reference/en/libraries/taskmanagerio/
 */

#include <Arduino.h>
#include <IoAbstraction.h>

// Functions declaration.
void onButtonTriggered(uint8_t pin, bool isHeldDown);

const pinid_t PUSH_BUTTON_PIN = 6;
bool isLedOn = false;

void setup() {
  Serial.begin(57600);
  Serial.println("START");

  // Declare the integrated LED on pin 13 as output.
  pinMode(LED_BUILTIN, OUTPUT);

  // Options: SWITCHES_NO_POLLING, SWITCHES_POLL_EVERYTHING,
  // SWITCHES_POLL_KEYS_ONLY (rotary encoders by interrupts, buttons by poll).
  // Last arg (true) is for integrated pull-up resistors.
  switches.init(ioUsingArduino(), SWITCHES_NO_POLLING, true);
  // If using a pull-down resistor instead, then use the arg `invertLogic =
  // true` (last arg).
  // switches.addSwitch(PUSH_BUTTON_PIN, onButtonTriggered, NO_REPEAT, true);
  switches.addSwitch(PUSH_BUTTON_PIN, onButtonTriggered, NO_REPEAT);
}

void onButtonTriggered(uint8_t pin, bool isHeldDown) {
  if (isHeldDown) {
    // IMP: mind that when holding a button, this function is invoked twice:
    //  - once, right after the press, with isHeldDown=false;
    //  - after 2 secs hold, with isHeldDown=true.
    // Hold down the button for 2 secs.
    Serial.println("HELD FOR 2 SECS");
    return;
  }

  Serial.println("PRESSED");
  isLedOn = !isLedOn;
  digitalWrite(LED_BUILTIN, isLedOn);
}

void loop() { taskManager.runLoop(); }
