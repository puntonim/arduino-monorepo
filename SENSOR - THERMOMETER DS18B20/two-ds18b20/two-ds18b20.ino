/* TWO DS18B20
 * ===========
 * Read the T value from 2 T sensors.
 *
 * Libs: 
 *  - DallasTemperature by Miles Burton https://docs.arduino.cc/libraries/dallastemperature/
 */

#include <OneWire.h>
#include <DallasTemperature.h>

const int PROBE_PIN = 4;

OneWire oneWire(PROBE_PIN);
DallasTemperature probes(&oneWire);
DeviceAddress probeAddress1;
DeviceAddress probeAddress2;
// If I know the address:
// const DeviceAddress probeAddress1 = {40, 255, 100, 31, 94, 120, 150, 103};
// const DeviceAddress probeAddress2 = {40, 255, 100, 31, 94, 127, 146, 104};

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
  if (!oneWire.search(probeAddress1)) Serial.println("Error: unable to find probes");
  Serial.println((String) "Found probe at address: " + probeAddress1[0] + " " + probeAddress1[1] + " " + probeAddress1[2] + " " + probeAddress1[3] + " " + probeAddress1[4] + " " + probeAddress1[5] + " " + probeAddress1[6] + " " + probeAddress1[7]);
  if (!oneWire.search(probeAddress2)) Serial.println("Error: unable to find probes");
  Serial.println((String) "Found probe at address: " + probeAddress2[0] + " " + probeAddress2[1] + " " + probeAddress2[2] + " " + probeAddress2[3] + " " + probeAddress2[4] + " " + probeAddress2[5] + " " + probeAddress2[6] + " " + probeAddress2[7]);
}

void loop() {
  // It would be better to use requestTemperaturesByAddress(ADDRESS) 2 times as
  //  we would avoid a delay when no sensor is detected (as the library blocks for
  //  ~1 sec waiting for any sensor). 
  probes.requestTemperatures();

  float tempC1 = probes.getTempC(probeAddress1);
  if (tempC1 == DEVICE_DISCONNECTED_C) Serial.println("Probe #1 error");
  Serial.println((String) "T1: " + tempC1);

  float tempC2 = probes.getTempC(probeAddress2);
  if (tempC2 == DEVICE_DISCONNECTED_C) Serial.println("Probe #2 error");
  Serial.println((String) "T2: " + tempC2);

  Serial.println((String) "getDeviceCount: " + probes.getDeviceCount());
  Serial.println((String) "isConnected: " + probes.isConnected(probeAddress1));
  Serial.println((String) "isConnected: " + probes.isConnected(probeAddress2));
  // Parasite mode should be avoided unless necessary, it means connecting the red and black
  //  wires both to ground. But you sould also need a cpacitor and maybe a mosfet if
  //  using temperature conversions: https://www.analog.com/media/en/technical-documentation/data-sheets/ds18b20.pdf
  Serial.println((String)"isParasitePowerMode: " + probes.isParasitePowerMode());
  Serial.println((String)"getTempCByIndex: " + probes.getTempCByIndex(0));
  Serial.println((String)"getHighAlarmTemp: " + probes.getHighAlarmTemp(probeAddress1));
  Serial.println((String)"getHighAlarmTemp: " + probes.getHighAlarmTemp(probeAddress2));
  Serial.println((String)"hasAlarm: " + probes.hasAlarm(probeAddress1));
  Serial.println((String)"hasAlarm: " + probes.hasAlarm(probeAddress2));
  
  Serial.println();
  delay(1000);
}
