/*
 * MULTIPLE PUSH BUTTONS WITH IOABSTRACTION
 * ========================================
 * Detect pushes of any of the 3 buttons using the lib IOAbstraction.
 * Also, detect when the button is held down for >2 secs.
 *
 * Note: push buttons are momentary switches.
 * Note: button connected with the integrated pull-up resistor.
 * Note: Arduino UNO R4 seems to have only 2 PINs that support interrupts
 *  pin 2 and 3, see:
 *  https://docs.arduino.cc/language-reference/en/functions/external-interrupts/attachInterrupt/
 *  however I proved that it works also on other PINs (like 6).
 *  BUT IT DOESN'T WORK WITH 3 BUTTONS!!
 *  SO, WITH 3 BUTTONS, YOU MUST USE POLLING.
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
 *  3x push button (using Arduino's internal pull-up resistor)
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

const unsigned short BUTTON_A_PIN = 2;
const unsigned short BUTTON_B_PIN = 3;
const unsigned short BUTTON_C_PIN = 4;

void setup() {
  Serial.begin(57600);
  Serial.println("START");

  // Options: SWITCHES_NO_POLLING, SWITCHES_POLL_EVERYTHING,
  // SWITCHES_POLL_KEYS_ONLY (rotary encoders by interrupts, buttons by poll).
  // Last arg (true) is for integrated pull-up resistors.
  // IMP: SINCE WE ARE USING 3 BUTTONS THEN WE HAVE TO USE POLLING (instead of
  //  interrupts because the limit in Arduino UNO R4 is 2 interrupts).
  switches.init(ioUsingArduino(), SWITCHES_POLL_KEYS_ONLY, true);

  // If using a pull-down resistor instead, then use the arg `invertLogic =
  // true` (last arg).
  // switches.addSwitch(PUSH_BUTTON_PIN, onButtonTriggered, NO_REPEAT, true);
  switches.addSwitch(BUTTON_A_PIN, onButtonTriggered, NO_REPEAT);
  switches.addSwitch(BUTTON_B_PIN, onButtonTriggered, NO_REPEAT);
  switches.addSwitch(BUTTON_C_PIN, onButtonTriggered, NO_REPEAT);
}

void onButtonTriggered(uint8_t pin, bool isHeldDown) {
  if (isHeldDown) {
    Serial.println((String) "BUTTON " + pin + " HELD FOR 2 SECS");
    return;
  }

  Serial.println((String) "BUTTON " + pin + " PRESSED");
}

void loop() { taskManager.runLoop(); }
