taskid_t blinkHeatingLedTaskId = TASKMGR_INVALIDID;
taskid_t blinkErrorLedTaskId = TASKMGR_INVALIDID;


class HeatingLedDevice {

private:
  bool _isOn = false;

public:
  void setup();
  void switchOn();
  void switchOff();
  void toggle();
  void startBlinking();
  void stopBlinking();
};

HeatingLedDevice heatingLedDevice;


class ErrorLedDevice {

private:
  bool _isOn = false;

public:
  void setup();
  void switchOn();
  void switchOff();
  void toggle();
  void startBlinking();
  void stopBlinking();
};

ErrorLedDevice errorLedDevice;
