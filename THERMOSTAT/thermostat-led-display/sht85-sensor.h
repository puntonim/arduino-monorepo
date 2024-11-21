// Note: SHT85 I2C address: 0x44.
SHTSensor sht(SHTSensor::SHT85);
// To use autodetect: SHTSensor sht;

enum struct Sht85SensorException {
  Success,
  SensorError,
};


class Sht85Sensor {

private:

public:
  void setup();
  void getData(float* data, enum Sht85SensorException &exc);
};

Sht85Sensor sht85Sensor;
