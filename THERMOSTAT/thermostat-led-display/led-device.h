taskid_t blinkHeatingLedTaskId = TASKMGR_INVALIDID;
taskid_t blinkErrorLedTaskId = TASKMGR_INVALIDID;


class HeatingLedDevice {

private:
  bool _isOn = false;
  const unsigned int _PIN = settings.HEATING_LED_PIN;
  const unsigned int _BRIGHTNESS_VALUE = settings.HEATING_LED_BRIGHTNESS_VALUE;

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
  const unsigned int _PIN = settings.ERROR_LED_PIN;
  const unsigned int _BRIGHTNESS_VALUE = settings.ERROR_LED_BRIGHTNESS_VALUE;

public:
  void setup();
  void switchOn();
  void switchOff();
  void toggle();
  void startBlinking();
  void stopBlinking();
};

ErrorLedDevice errorLedDevice;


class DomainLedDevice {

private:
  bool _isOn = false;
  const unsigned int _PIN = settings.DOMAIN_LED_PIN;
  const unsigned int _BRIGHTNESS_VALUE = settings.DOMAIN_LED_BRIGHTNESS_VALUE;

public:
  void setup();
  void switchOn();
  void switchOff();
};

DomainLedDevice domainLedDevice;