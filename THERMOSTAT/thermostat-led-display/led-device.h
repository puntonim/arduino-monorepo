taskid_t blinkHeatingLedTaskId = TASKMGR_INVALIDID;
taskid_t blinkErrorLedTaskId = TASKMGR_INVALIDID;


class HeatingLedDevice {

private:
  bool _isOn = false;
  const unsigned short _PIN = settings.HEATING_LED_PIN;
  const unsigned short _BRIGHTNESS_VALUE = settings.HEATING_LED_BRIGHTNESS_VALUE;

public:
  void setup();
  void switchOn();
  void switchOff();
  void toggle();
  void startBlinking();
  void stopBlinking();
};


class ErrorLedDevice {

private:
  bool _isOn = false;
  const unsigned short _PIN = settings.ERROR_LED_PIN;
  const unsigned short _BRIGHTNESS_VALUE = settings.ERROR_LED_BRIGHTNESS_VALUE;

public:
  void setup();
  void switchOn();
  void switchOff();
  void toggle();
  void startBlinking();
  void stopBlinking();
};


class DomainLedDevice {

private:
  bool _isOn = false;
  const unsigned short _PIN = settings.DOMAIN_LED_PIN;
  const unsigned short _BRIGHTNESS_VALUE = settings.DOMAIN_LED_BRIGHTNESS_VALUE;

public:
  void setup();
  void switchOn();
  void switchOff();
};
