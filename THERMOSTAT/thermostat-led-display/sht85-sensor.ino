void Sht85Sensor::setup() {
  // Wire.begin();
  // It should return a bool, but I tested it disconnecting wires and it always
  //  returns 0;
  sht.init();
  // SHT_ACCURACY_HIGH: highest repeatability at the cost of slower measurement.
  sht.setAccuracy(SHTSensor::SHT_ACCURACY_HIGH);
}

// Note: at the moment the sensor SHT85 is only read by the display, so its error status is updated
//  only when the display is ON. This means that if the errorLed has been triggered by an error on
//   the SHT85 sensor, then the errorLed status can be delayed until the display is ON again.

void Sht85Sensor::getData(float* data, enum Sht85SensorException &exc) {
  if (sht.readSample()) {
    data[0] = sht.getTemperature();
    data[1] = sht.getHumidity();
    exc = Sht85SensorException::Success;
    errorManager.removeSht85SensorError();
  } else {
#if IS_DEBUG == true
    Serial.println((String) "Sht85Sensor - error reading data");
#endif
    data[0] = SENSOR_ERROR;
    data[1] = SENSOR_ERROR;
    exc = Sht85SensorException::SensorError;
    errorManager.addSht85SensorError();
  }
}
