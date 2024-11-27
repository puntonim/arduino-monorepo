// Note: SHT85 I2C address: 0x44.
SHTSensor sht(SHTSensor::SHT85);
// To use autodetect: SHTSensor sht;

struct Sht85Data {
  float temperature;
  float humidity;
};

enum struct Sht85SensorException {
  Success,
  SensorError,
};


class Sht85Sensor {

private:
  unsigned long _lastDataTs = 0;
  float _cachedTemperature;
  float _cachedHumidity;

public:
  void setup();
  struct Sht85Data getData(enum Sht85SensorException& exc);
};
