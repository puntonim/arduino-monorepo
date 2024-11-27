// Note: SHT85 I2C address: 0x44.
SHTSensor sht(SHTSensor::SHT85);
// To use autodetect: SHTSensor sht;

enum struct Sht85SensorException {
  Success,
  SensorError,
};


class Sht85Sensor {

private:
  unsigned long _lastDataTs = millis();
  float _cachedTemperature;
  float _cachedHumidity;

public:
  void setup();
  void getData(float* data, enum Sht85SensorException& exc);
};
