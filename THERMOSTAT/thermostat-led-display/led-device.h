const unsigned int HEATING_LED_PIN = 6;
const unsigned int HEATING_LED_BRIGHTNESS_VALUE = 30;
const unsigned int ERROR_LED_PIN = 9;

taskid_t toggleErrorLedTaskId = TASKMGR_INVALIDID;


class HeatingLedDevice {

private:
  bool _isOn = false;

public:
  void setup();
  void switchOn();
  void switchOff();
  void toggle();
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
