OneWire oneWire(settings.DS18B20_SENSOR_PIN);
DallasTemperature probes(&oneWire);


enum struct Ds18b20SensorException {
  Success,
  SensorError,
};


class Ds18b20Sensor {

private:

public:
  void setup();
  float getData(enum Ds18b20SensorException &exc);
};

Ds18b20Sensor ds18b20Sensor;
