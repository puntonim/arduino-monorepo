/* SHT-85
 * ======
 * Print to console T and humidity.
 *
 * Libs: 
 *  - arduino-sht by Johannes Winkelmann, Andreas Brauchli https://github.com/Sensirion/arduino-sht
 */

#include <Wire.h>
#include <SHTSensor.h>

// Note: SHT85 I2C address: 0x44.
SHTSensor sht(SHTSensor::SHT85);
// To use autodetect: SHTSensor sht;

void setup() {
  Wire.begin();
  Serial.begin(57600);
  while (!Serial) delay(100);

  while (!sht.init()) {
    Serial.print("SHT85 init error");
    delay(2000);
  }
  // SHT_ACCURACY_HIGH: highest repeatability at the cost of slower measurement.
  sht.setAccuracy(SHTSensor::SHT_ACCURACY_HIGH);
}

void loop() {
  if (sht.readSample()) {
    Serial.print("H: ");
    Serial.print(sht.getHumidity());  // 2 decimals.
    Serial.print("% - T: ");
    Serial.print(sht.getTemperature());  // 2 decimals.
    Serial.println("°C");
  } else {
    Serial.println("Error reading data from SHT85");
  }

  delay(5 * 1000);
}
