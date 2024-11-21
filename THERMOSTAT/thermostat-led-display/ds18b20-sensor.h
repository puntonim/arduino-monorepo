const unsigned int PROBE_PIN = 4;

OneWire oneWire(PROBE_PIN);
DallasTemperature probes(&oneWire);
const DeviceAddress PROBE_ADDRESS = { 40, 255, 100, 31, 94, 120, 150, 103 };


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
