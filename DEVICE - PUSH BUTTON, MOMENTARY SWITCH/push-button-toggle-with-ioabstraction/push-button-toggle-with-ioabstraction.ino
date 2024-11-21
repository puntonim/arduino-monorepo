/*
 * PUSH BUTTON TOGGLE WITH IOABSTRACTION
 * =====================================
 * Detect the push of a button and toggle the state of the bultin LED
 *  using the lib IOAbstraction.
 * Note: push buttons are momentary switches.
 * Note: Arduino UNO R4 has only 2 PINs that support interrupts, see: https://docs.arduino.cc/language-reference/en/functions/external-interrupts/attachInterrupt/
 * Note: after pressing the button 1 non-repeating task is scheduled with TaskManagerIO and lives forever.
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
 *  1x push button
 * 
 * Docs:
 *  - https://tcmenu.github.io/documentation/arduino-libraries/io-abstraction/
 *
 * Libs:
 *  - IoAbstraction by davetecc https://reference.arduino.cc/reference/en/libraries/ioabstraction/
 *  - TaskManagerIO by davetecc https://reference.arduino.cc/reference/en/libraries/taskmanagerio/
 */

#include <IoAbstraction.h>

const int PUSH_BUTTON_PIN = 2;
// When using polling instead of interrupts (SWITCHES_POLL_EVERYTHING) the led is ON on boot,
//  so I should instead use this: bool isLedOff = false;
bool isLedOn = false;

void setup() {
  Serial.begin(57600);
  // Declare the integrated LED as output;
  pinMode(LED_BUILTIN, OUTPUT);

  // Options: SWITCHES_NO_POLLING, SWITCHES_POLL_EVERYTHING, SWITCHES_POLL_KEYS_ONLY.
  switches.init(ioUsingArduino(), SWITCHES_NO_POLLING, true);
  // Note: after pressing the button 1 non-repeating task is scheduled with TaskManagerIO
  //  and lives forever.
  switches.addSwitch(PUSH_BUTTON_PIN, onButtonTriggered, NO_REPEAT);
}

void onButtonTriggered(uint8_t pin, bool isHeldDown) {
  // Serial.println((String) "X" + isHeldDown);
  if (isHeldDown) return;

  Serial.println("PRESSED");
  isLedOn = !isLedOn;
  digitalWrite(LED_BUILTIN, isLedOn);
}

void loop() {
  // Debug: print scheduled with TaskManagerIO.
  // After pressing the button 1 non-repeating task is scheduled with TaskManagerIO
  //  and lives forever. 
  printDebugTasks();
  
  taskManager.runLoop();
}
