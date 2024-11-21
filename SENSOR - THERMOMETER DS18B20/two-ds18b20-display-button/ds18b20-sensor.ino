const int PROBE_PIN = 4;

OneWire oneWire(PROBE_PIN);
DallasTemperature probes(&oneWire);
const DeviceAddress PROBE_ADDRESS1 = { 40, 255, 100, 31, 94, 120, 150, 103 };
const DeviceAddress PROBE_ADDRESS2 = { 40, 255, 100, 31, 94, 127, 146, 104 };

void setupDs18b20() {
  probes.begin();
  // The resolution of the temperature sensor is user-configurable to 9, 10, 11, or 12 bits,
  //  corresponding to increments of 0.5°C, 0.25°C, 0.125°C, and 0.0625°C, respectively.
  // The default resolution at power-up is 12-bit.
  // Src: https://www.analog.com/media/en/technical-documentation/data-sheets/ds18b20.pdf
  // However the default resolution used by the DallasTemperature is 9 bit.
  probes.setResolution(12);
}

float getDs18b20Data(float* data) {
  // It would be better to use requestTemperaturesByAddress(ADDRESS) 2 times as
  //  we would avoid a delay when no sensor is detected (as the library blocks for
  //  ~1 sec waiting for any sensor).
  probes.requestTemperatures();

  float tempC = probes.getTempC(PROBE_ADDRESS1);
  if (tempC == DEVICE_DISCONNECTED_C) data[0] = SENSOR_ERROR;
  else data[0] = tempC;

  tempC = probes.getTempC(PROBE_ADDRESS2);
  if (tempC == DEVICE_DISCONNECTED_C) data[1] = SENSOR_ERROR;
  else data[1] = tempC;
}
