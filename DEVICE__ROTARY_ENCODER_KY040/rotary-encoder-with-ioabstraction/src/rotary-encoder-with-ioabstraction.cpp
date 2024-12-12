/*
 * ROTARY ENCODER WITH IOABSTRACTION
 * =================================
 * Detect the rotation of a rotary encoder and print the value.
 * Also, detect the push and toggle the state of the bultin LED.
 * Using the lib IOAbstraction.
 * Also, detect when the button is held down for >2 secs.
 *
 * Note: rotary encoder connected with the integrated pull-up resistor.
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
 *  1x KY-040 rotary encoder (using Arduino's internal pull-up resistor)
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

const pinid_t ROTARY_CLK_PIN = 2;  // PIN A.
const pinid_t ROTARY_DT_PIN = 3;   // PIN B.
const pinid_t ROTARY_SW_PIN = 12;  // PIN switch.

// Functions declaration.
void onEncoderChange(const int newValue);
void onButtonTriggered(const pinid_t pin, const bool isHeldDown);

bool isLedOn = false;

void setup() {
  Serial.begin(57600);
  Serial.println("START");

  // Declare the integrated LED on pin 13 as output.
  pinMode(LED_BUILTIN, OUTPUT);

  // Setup the rotary press as a regular switch button.
  // Options: SWITCHES_NO_POLLING, SWITCHES_POLL_EVERYTHING,
  // SWITCHES_POLL_KEYS_ONLY (rotary encoders by interrupts, buttons by poll).
  // Mind that there is a limit in the number of devices that can work with
  //  interrupts simultaneously.
  // Last arg (true) is for integrated pull-up resistors.
  switches.init(ioUsingArduino(), SWITCHES_NO_POLLING, true);
  switches.addSwitch(ROTARY_SW_PIN, onButtonTriggered, NO_REPEAT);

  // Seturp the rotary encoder.
  // Options for HA acceleration mode: HWACCEL_SLOWER, HWACCEL_REGULAR,
  // HWACCEL_NONE.
  // Note: switch the order of the params ROTARY_DT_PIN and ROTARY_CLK_PIN
  //  if you want to invert the clockwise-counterclockw. behavior.
  auto rotary = new HardwareRotaryEncoder(ROTARY_DT_PIN, ROTARY_CLK_PIN,
                                          onEncoderChange, HWACCEL_NONE);
  // Indexed array to hold all the rotaries.
  const u_int8_t encoderSlot = 0;
  switches.setEncoder(encoderSlot, rotary);
  // Configure the rotary.
  rotary->changePrecision(40,     // max value.
                          20,     // starting value.
                          false,  // Wrap around after hitting min and max.
                          1       // step size.
  );
}

void onEncoderChange(const int value) {
  Serial.print((String)"Encoder change: " + value);
}

void onButtonTriggered(const uint8_t pin, const bool isHeldDown) {
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
