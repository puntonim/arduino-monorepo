/*
 * IR RECEIVER TSOP4838
 * ====================
 * Read the IR command sent by a remote working at 38 kHz (the most used).
 *
 * Devices:
 *  1x TSOP4838 IR receiver
 *   https://www.mouser.it/ProductDetail/Vishay-Semiconductors/TSOP4838?qs=yGXpg7PJZCiwO12kec0Sug%3D%3D
 *  1x 2.2 uF capacitor
 *
 * Workshop:
 *  - ttps://dronebotworkshop.com/ir-remotes/
 *
 * Libs:
 *  - IRremote by Armin Joachimsmeyer
 *    https://docs.arduino.cc/libraries/irremote/
 */

// Src:
// https://github.com/Arduino-IRremote/Arduino-IRremote/blob/master/examples/SimpleReceiver/SimpleReceiver.ino

#include <Arduino.h>

#include <IRremote.hpp>

const unsigned short IR_RECEIVER_PIN = 10;

void setup() {
  Serial.begin(57600);
  Serial.println(F("START " __FILE__ " from " __DATE__
                   "\r\nUsing library version " VERSION_IRREMOTE));

  IrReceiver.begin(IR_RECEIVER_PIN, ENABLE_LED_FEEDBACK);

  Serial.print("Ready to receive IR signals of protocols: ");
  printActiveIRProtocols(&Serial);
  Serial.println();
  Serial.println((String) "IR receiver on PIN " + IR_RECEIVER_PIN);
}

void loop() {
  if (IrReceiver.decode()) {
    // Command: `IrReceiver.decodedIRData.command`.
    // Address: `IrReceiver.decodedIRData.address`.
    // Raw data: `IrReceiver.decodedIRData.decodedRawData`.

    if (IrReceiver.decodedIRData.protocol == UNKNOWN) {
      Serial.println(
          "Received noise or an unknown (or not yet enabled) protocol");
      IrReceiver.printIRResultRawFormatted(&Serial, true);
      IrReceiver.resume();
    } else {
      IrReceiver.resume();
      IrReceiver.printIRResultShort(&Serial);
      IrReceiver.printIRSendUsage(&Serial);
    }
    Serial.println();

    if (IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT) {
      Serial.println(
          "Repeat received. Here you can repeat the same action as before");
    } else {
      if (IrReceiver.decodedIRData.command == 0x10) {
        Serial.println("Received command 0x10");
      } else if (IrReceiver.decodedIRData.command == 0x11) {
        Serial.println("Received command 0x11");
      }
    }
  }
}

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
