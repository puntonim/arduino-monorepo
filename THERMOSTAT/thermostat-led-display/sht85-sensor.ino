void Sht85Sensor::setup() {
  // Wire.begin();
  // It should return a bool, but I tested it disconnecting wires and it always
  //  returns 0;
  sht.init();
  // SHT_ACCURACY_HIGH: highest repeatability at the cost of slower measurement.
  sht.setAccuracy(SHTSensor::SHT_ACCURACY_HIGH);
}

void Sht85Sensor::getData(float* data, enum Sht85SensorException &exc) {
  if (sht.readSample()) {
    data[0] = sht.getTemperature();
    data[1] = sht.getHumidity();
    exc = Sht85SensorException::Success;
  } else {
    data[0] = SENSOR_ERROR;
    data[1] = SENSOR_ERROR;
    exc = Sht85SensorException::SensorError;
  }
}
