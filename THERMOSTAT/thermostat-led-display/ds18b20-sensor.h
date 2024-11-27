OneWire oneWire(settings.DS18B20_SENSOR_PIN);
DallasTemperature probes(&oneWire);


enum struct Ds18b20SensorException {
  Success,
  SensorError,
};


class Ds18b20Sensor {

private:
   unsigned long _lastDataTs = 0;
   float _cachedData;

public:
  void setup();
  float getData(enum Ds18b20SensorException &exc);
};
