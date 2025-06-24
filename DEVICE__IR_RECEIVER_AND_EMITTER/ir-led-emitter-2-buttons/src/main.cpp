/*
 * IR LED EMITTER TSUS5402 WITH 2 BUTTONS
 * ======================================
 * Send IR signals with the IR LED TSUS5402, controlled with 2 buttons.
 *
 * Devices:
 *  1x TSUS5402 IR LED (emitter)
 *   https://4donline.ihs.com/images/VipMasterIC/IC/VISH/VISH-S-A0012362795/VISH-S-A0012397615-1.pdf?hkey=CECEF36DEECDED6468708AAF2E19C0C6
 *  1x transistor NPN 2N2222 A
 *   https://www.onsemi.com/pdf/datasheet/p2n2222a-d.pdf
 *  1x resistor 33R
 *  1x resistor 1k
 *   2x push buttons
 *
 * Workshop:
 *  - https://dronebotworkshop.com/ir-remotes/
 *
 * Libs:
 *  - IRremote by Armin Joachimsmeyer
 *    https://docs.arduino.cc/libraries/irremote/
 */

// Src:
// https://github.com/Arduino-IRremote/Arduino-IRremote/blob/master/examples/SimpleSender/SimpleSender.ino

/*
Codes detected for my A/C remote Mizushi.

ON or OFF 31 deg
Protocol=PulseDistance Raw-Data=0xD2070683 48 bits LSB first Gap=3276750us
Duration=83250us Send on a 32 bit platform with:
IrSender.sendPulseDistanceWidth(38, 9050, 4450, 650, 1650, 650, 550, 0xD2070683,
48, PROTOCOL_IS_LSB_FIRST, <RepeatPeriodMillis>, <numberOfRepeats>);

UP or DOWN to 31 deg
Protocol=PulseDistance Raw-Data=0xD2030683 48 bits LSB first Gap=2059200us
Duration=82150us Send on a 32 bit platform with:
IrSender.sendPulseDistanceWidth(38, 9100, 4450, 650, 1600, 650, 550, 0xD2030683,
48, PROTOCOL_IS_LSB_FIRST, <RepeatPeriodMillis>, <numberOfRepeats>);

UP or DOWN to 22 deg
Protocol=PulseDistance Raw-Data=0x42030683 48 bits LSB first Gap=1683550us
Duration=79900us Send on a 32 bit platform with:
IrSender.sendPulseDistanceWidth(38, 9050, 4450, 650, 1600, 650, 500, 0x42030683,
48, PROTOCOL_IS_LSB_FIRST, <RepeatPeriodMillis>, <numberOfRepeats>);
*/

// Disables static receiver code, like receive timer ISR handler and static
//  IRReceiver and irparams data. Saves 450 bytes program memory and 269 bytes
//  RAM. This is due to a bug in RISC-V compiler, which requires unused function
//  sections.
#if !defined(ARDUINO_ESP32C3_DEV)
#define DISABLE_CODE_FOR_RECEIVER
#endif

// Disable carrier PWM generation in software and use (restricted) hardware PWM.
/// #define SEND_PWM_BY_TIMER
// Use no carrier PWM, just simulate an active low receiver signal. Overrides
// SEND_PWM_BY_TIMER definition.
/// #define USE_NO_SEND_PWM

#include <Arduino.h>
#include <IoAbstraction.h>

#include <IRremote.hpp>

// Functions declaration.
void onButtonTriggered(uint8_t pin, bool isHeldDown);

const unsigned short BUTTON_A_PIN = 11;
const unsigned short BUTTON_B_PIN = 10;
const unsigned short IR_LED_PIN = 2;

void setup() {
  Serial.begin(57600);
  unsigned short i = 30;
  while (!Serial && i--) delay(1);
  Serial.println(F("START " __FILE__ " from " __DATE__
                   "\nUsing library version " VERSION_IRREMOTE));

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

  IrSender.begin(IR_LED_PIN);
  // Disable feedback on the default LED.
  disableLEDFeedback();
}

void onButtonTriggered(uint8_t pin, bool isHeldDown) {
  if (isHeldDown) {
    Serial.println("Sending command to ON/OFF 31 deg...");
    IrSender.sendPulseDistanceWidth(38, 9050, 4450, 650, 1600, 650, 500,
                                    0xD2070683, 48, PROTOCOL_IS_LSB_FIRST, 0,
                                    NO_REPEATS);
    return;
  }

  if (pin == BUTTON_A_PIN) {
    Serial.println("Sending command to 31 deg...");
    IrSender.sendPulseDistanceWidth(38, 9100, 4450, 650, 1600, 650, 550,
                                    0xD2030683, 48, PROTOCOL_IS_LSB_FIRST, 0,
                                    NO_REPEATS);
    return;
  }

  if (pin == BUTTON_B_PIN) {
    Serial.println("Sending command to 22 deg...");
    IrSender.sendPulseDistanceWidth(38, 9050, 4450, 650, 1600, 650, 500,
                                    0x42030683, 48, PROTOCOL_IS_LSB_FIRST, 0,
                                    NO_REPEATS);
    return;
  }
}

void loop() { taskManager.runLoop(); }
