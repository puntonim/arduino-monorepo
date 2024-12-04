/* DS18B20
 * =======
 * Read the T value.
 * Note: in the commented code at the end of the file I used another lib, less popular: ds18b20.
 *
 * Libs: 
 *  - DallasTemperature by Miles Burton https://docs.arduino.cc/libraries/dallastemperature/
 */

#include <OneWire.h>
#include <DallasTemperature.h>

const int PROBE_PIN = 4;

OneWire oneWire(PROBE_PIN);
DallasTemperature probes(&oneWire);
DeviceAddress probeAddress;
// If I know the address:
// const DeviceAddress probeAddress = {40, 255, 100, 31, 94, 120, 150, 103};

void setup() {
  Serial.begin(57600);
  probes.begin();
  // The resolution of the temperature sensor is user-configurable to 9, 10, 11, or 12 bits,
  //  corresponding to increments of 0.5°C, 0.25°C, 0.125°C, and 0.0625°C, respectively.
  // The default resolution at power-up is 12-bit.
  // Src: https://www.analog.com/media/en/technical-documentation/data-sheets/ds18b20.pdf
  // However the default resolution used by the DallasTemperature is 9 bit.
  probes.setResolution(9);

  // Must be called before search().
  oneWire.reset_search();
  if (!oneWire.search(probeAddress)) Serial.println("Error: unable to find probes");
  Serial.println((String) "Found probe at address: " + probeAddress[0] + " " + probeAddress[1] + " " + probeAddress[2] + " " + probeAddress[3] + " " + probeAddress[4] + " " + probeAddress[5] + " " + probeAddress[6] + " " + probeAddress[7]);
}

void loop() {
  // probes.requestTemperatures();
  // By using requestTemperaturesByAddress(ADDRESS) we avoid a delay when
  //  no sensor is detected (as the library blocks for ~1sec waiting for any sensor). 
  probes.requestTemperaturesByAddress(probeAddress);

  float tempC = probes.getTempC(probeAddress);
  if (tempC == DEVICE_DISCONNECTED_C) Serial.println("Probe #1 error");
  Serial.println((String) "T: " + tempC);

  Serial.println((String) "getDeviceCount: " + probes.getDeviceCount());
  Serial.println((String) "isConnected: " + probes.isConnected(probeAddress));
  // Parasite mode should be avoided unless necessary, it means connecting the red and black
  //  wires both to ground. But you sould also need a cpacitor and maybe a mosfet if
  //  using temperature conversions: https://www.analog.com/media/en/technical-documentation/data-sheets/ds18b20.pdf
  Serial.println((String)"isParasitePowerMode: " + probes.isParasitePowerMode());
  Serial.println((String)"getTempCByIndex: " + probes.getTempCByIndex(0));
  Serial.println((String)"getHighAlarmTemp: " + probes.getHighAlarmTemp(probeAddress));
  Serial.println((String)"hasAlarm: " + probes.hasAlarm(probeAddress));
  
  Serial.println();
  delay(1000);
}



// // This would be the code using the less popular lib ds18b20: https://docs.arduino.cc/libraries/ds18b20/
// //    whcih also requires the lib OneWire to be manually installed: https://docs.arduino.cc/libraries/onewire/

// #include <DS18B20.h>

// const int PROBE_PIN = 4;
// DS18B20 ds(PROBE_PIN);
// uint8_t probeAddress[8];
// // If known: uint8_t probeAddress[] = {40, 255, 100, 31, 94, 120, 150, 103};
// uint8_t probe;

// void setup() {
//   Serial.begin(57600);

//   while (ds.selectNext()) {
//     ds.getAddress(probeAddress);
//     const auto a = probeAddress;
//     Serial.println((String) "Found probe at address: " + a[0] + " " + a[1] + " " + a[2] + " " + a[3] + " " + a[4] + " " + a[5] + " " + a[6] + " " + a[7]);
//   }
//   probe = ds.select(probeAddress);
//   if (!probe) Serial.println("Probe not found!");
// }

// void loop() {
//   Serial.println((String) "T: " + ds.getTempC());

//   Serial.println((String) "Res: " + ds.getResolution());
//   Serial.println((String) "Power: " + ds.getPowerMode());
//   Serial.println((String) "Fam: " + ds.getFamilyCode());
//   Serial.println((String) "Alarm: " + ds.hasAlarm());
  
//   Serial.println();
//   delay(1000);
// }

