const int PROBE_PIN = 4;

OneWire oneWire(PROBE_PIN);
DallasTemperature probes(&oneWire);
const DeviceAddress PROBE_ADDRESS = { 40, 255, 100, 31, 94, 120, 150, 103 };

void setupDs18b20() {
  probes.begin();
  // The resolution of the temperature sensor is user-configurable to 9, 10, 11, or 12 bits,
  //  corresponding to increments of 0.5°C, 0.25°C, 0.125°C, and 0.0625°C, respectively.
  // The default resolution at power-up is 12-bit.
  // Src: https://www.analog.com/media/en/technical-documentation/data-sheets/ds18b20.pdf
  // However the default resolution used by the DallasTemperature is 9 bit.
  probes.setResolution(12);
}

float getDs18b20Data() {
  // probes.requestTemperatures();
  // By using requestTemperaturesByAddress(ADDRESS) we avoid a delay when
  //  no sensor is detected (as the library blocks for ~1sec waiting for any sensor). 
  probes.requestTemperaturesByAddress(PROBE_ADDRESS);
  float tempC = probes.getTempC(PROBE_ADDRESS);
  if (tempC == DEVICE_DISCONNECTED_C) return SENSOR_ERROR;
  return tempC;
}
