void Ds18b20Sensor::setup() {
  probes.begin();
  // The resolution of the temperature sensor is user-configurable to 9, 10, 11, or 12 bits,
  //  corresponding to increments of 0.5°C, 0.25°C, 0.125°C, and 0.0625°C, respectively.
  // The default resolution at power-up is 12-bit.
  // Src: https://www.analog.com/media/en/technical-documentation/data-sheets/ds18b20.pdf
  // However the default resolution used by the DallasTemperature is 9 bit.
  probes.setResolution(12);
}

float Ds18b20Sensor::getData(enum Ds18b20SensorException &exc) {
  // probes.requestTemperatures();
  // By using requestTemperaturesByAddress(ADDRESS) we avoid a delay when
  //  no sensor is detected (as the library blocks for ~1sec waiting for any sensor). 
  probes.requestTemperaturesByAddress(settings.DS18B20_SENSOR_ADDRESS);

  float tempC = probes.getTempC(settings.DS18B20_SENSOR_ADDRESS);
  if (tempC == DEVICE_DISCONNECTED_C) {
    exc = Ds18b20SensorException::SensorError;
    return SENSOR_ERROR;
  }

  exc = Ds18b20SensorException::Success;
  return tempC;
}
