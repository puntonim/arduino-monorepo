/*
 * Scan the I2C interface and print the address of any connected device.
 *
 * Src: https://playground.arduino.cc/Main/I2cScanner/
 */

#include <Wire.h>
#define WIRE Wire

void setup() {
  WIRE.begin();
  Serial.begin(57600);
  while (!Serial) delay(100);
  Serial.println("I2C Scanner - print the address of any connected device");
}


void loop() {
  byte error, address;
  int nDevices;

  Serial.println("Scanning...");

  nDevices = 0;
  for(address = 1; address < 127; address++ ) {
    // The i2c_scanner uses the return value of `Write.endTransmisstion` to see if
    //  a device did acknowledge to the address.
    WIRE.beginTransmission(address);
    error = WIRE.endTransmission();

    if (error == 0) {
      Serial.print("I2C device found at address: 0x");
      if (address<16) Serial.print("0");
      Serial.println(address,HEX);
      nDevices++;
    } else if (error==4) {
      Serial.print("Unknown error at address: 0x");
      if (address<16) Serial.print("0");
      Serial.println(address,HEX);
    }
  }

  if (nDevices == 0) Serial.println("No I2C devices found");
  else Serial.println("done");

  delay(5000);
}
