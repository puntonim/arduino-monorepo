/*
 * MULTIPLE ROTARY ENCODERS WITH IOABSTRACTION
 * ===========================================
 * Detect the rotation of any of the 2 rotary encoders and print the value.
 * Also, detect the push and toggle the state of the bultin LED.
 * Using the lib IOAbstraction.
 * Also, detect when the button is held down for >2 secs.
 *
 * Note: rotary encoders connected with the integrated pull-up resistor.
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
 *  2x KY-040 rotary encoder (using Arduino's internal pull-up resistor)
 *
 * Docs:
 *  - https://tcmenu.github.io/documentation/arduino-libraries/io-abstraction/
 *
 * Libs:
 *  - IoAbstraction by davetecc
 *    https://reference.arduino.cc/reference/en/libraries/ioabstraction/
 */

#include <Arduino.h>
#include <IoAbstraction.h>

const pinid_t ROTARY_A_CLK_PIN = 2;  // PIN A.
const pinid_t ROTARY_A_DT_PIN = 3;   // PIN B.
const pinid_t ROTARY_A_SW_PIN = 4;   // PIN switch.
const pinid_t ROTARY_B_CLK_PIN = 5;  // PIN A.
const pinid_t ROTARY_B_DT_PIN = 6;   // PIN B.
const pinid_t ROTARY_B_SW_PIN = 7;   // PIN switch.

// Functions declaration.
void onEncoderAChange(const int newValue);
void onEncoderBChange(const int newValue);
void onButtonTriggered(const pinid_t pin, const bool isHeldDown);

bool isLedOn = false;

void setup() {
  Serial.begin(57600);
  Serial.println("START");

  // Declare the integrated LED on pin 13 as output.
  pinMode(LED_BUILTIN, OUTPUT);

  // Setup the rotary encoder press as a regular switch button.
  // Options: SWITCHES_NO_POLLING, SWITCHES_POLL_EVERYTHING,
  // SWITCHES_POLL_KEYS_ONLY (rotary encoders by interrupts, buttons by poll).
  // Mind that there is a limit in the number of devices that can work with
  //  interrupts simultaneously. If using SWITCHES_NO_POLLING with 2 encoders
  //  then the button press does not work, while SWITCHES_POLL_KEYS_ONLY works.
  // Last arg (true) is for integrated pull-up resistors.
  switches.init(ioUsingArduino(), SWITCHES_POLL_KEYS_ONLY, true);
  switches.addSwitch(ROTARY_A_SW_PIN, onButtonTriggered, NO_REPEAT);
  switches.addSwitch(ROTARY_B_SW_PIN, onButtonTriggered, NO_REPEAT);

  // Setup the rotary encoder rotation.
  // Options for HA acceleration mode: HWACCEL_SLOWER, HWACCEL_REGULAR,
  // HWACCEL_NONE.
  // Note: switch the order of the params ROTARY_DT_PIN and ROTARY_CLK_PIN
  //  if you want to invert the clockwise-counterclockwise behavior.
  auto rotaryA = new HardwareRotaryEncoder(ROTARY_A_DT_PIN, ROTARY_A_CLK_PIN,
                                           onEncoderAChange, HWACCEL_NONE);
  auto rotaryB = new HardwareRotaryEncoder(ROTARY_B_DT_PIN, ROTARY_B_CLK_PIN,
                                           onEncoderBChange, HWACCEL_NONE);
  // Indexed array to hold all the rotaries.
  const u_int8_t encoderSlot = 0;
  switches.setEncoder(encoderSlot, rotaryA);
  switches.setEncoder(encoderSlot + 1, rotaryB);
  // Configure the rotary.
  rotaryA->changePrecision(40,     // max value.
                           20,     // starting value.
                           false,  // Wrap around after hitting min and max.
                           1       // step size.
  );
  // Using maxValue=0 causes the callback to get 1 on every clockwise rotation
  //  and -1 on every counterclockwise rotation.
  rotaryB->changePrecision(0,      // max value.
                           0,      // starting value.
                           false,  // Wrap around after hitting min and max.
                           1       // step size.
  );

  // To reset to the initial value.
  // rotaryA->setCurrentReading(20);
}

void onEncoderAChange(const int value) {
  Serial.println((String) "Encoder A change: " + value);
}
void onEncoderBChange(const int value) {
  Serial.println((String) "Encoder B change: " + value);
}

void onButtonTriggered(const uint8_t pin, const bool isHeldDown) {
  if (isHeldDown) {
    // IMP: mind that when holding a button, this function is invoked twice:
    //  - once, right after the press, with isHeldDown=false;
    //  - after 2 secs hold, with isHeldDown=true.
    // Hold down the button for 2 secs.
    Serial.println((String) "HELD FOR 2 SECS: " + pin);
    return;
  }

  Serial.println((String) "PRESSED: " + pin);
  isLedOn = !isLedOn;
  digitalWrite(LED_BUILTIN, isLedOn);
}

void loop() { taskManager.runLoop(); }
