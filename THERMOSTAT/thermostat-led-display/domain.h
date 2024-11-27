#define SENSOR_ERROR -127

taskid_t runTaskId = TASKMGR_INVALIDID;


class Domain {

private:
  bool _isHeatingOn = false;
  bool _checkForTemperature();
  bool _checkForTimer();
  void _switchHeatingOn();
  void _switchHeatingOff();

public:
  void setup();
  void run();
};

// Declare all global singletons.
Domain domain;

ButtonDevice buttonDevice;
DisplayDevice displayDevice;

Ds18b20Sensor ds18b20Sensor;
Sht85Sensor sht85Sensor;

HeatingLedDevice heatingLedDevice;
ErrorLedDevice errorLedDevice;
DomainLedDevice domainLedDevice;

time_utils::Timer timer;
ErrorManager errorManager;
PubSub pubSub;
