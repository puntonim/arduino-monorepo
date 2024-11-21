#define SENSOR_ERROR -127

taskid_t checkPeriodicallyTaskId = TASKMGR_INVALIDID;


class Domain {

private:
  bool _isHeatingOn = false;
  void _checkTemperature();
  void _switchHeatingOn();
  void _switchHeatingOff();

public:
  void setup();
  void run();
  bool isHeatingOn();
};

Domain domain;
