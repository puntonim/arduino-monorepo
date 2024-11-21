// Note: SHT85 I2C address: 0x44.
SHTSensor sht(SHTSensor::SHT85);
// To use autodetect: SHTSensor sht;

void setupSht85() {
  // Wire.begin();
  // It should return a bool, but I tested it disconnecting wires and it always
  //  returns 0;
  sht.init();
  // SHT_ACCURACY_HIGH: highest repeatability at the cost of slower measurement.
  sht.setAccuracy(SHTSensor::SHT_ACCURACY_HIGH);
}

void getSht85Data(float* data) {
  if (sht.readSample()) {
    data[0] = sht.getTemperature();
    data[1] = sht.getHumidity();
  } else {
    data[0] = SENSOR_ERROR;
    data[1] = SENSOR_ERROR;
  }
}