taskid_t toggleErrorLedTaskId = TASKMGR_INVALIDID;


class HeatingLedDevice {

private:
  bool _isOn = false;
  // Smart pointers to data ithe led depends on.
  // Note: by smart pointer e not by reference because references would require
  //  to be initialized in the contructor, but `heatingLedDevice` is a singleton with
  //  global scope and cannot know domain._isHeatingOn at construction time.
  //  Also, using smart pointers is a better solution.
  std::shared_ptr<const bool> _isHeatingOnPointer;  // `const` so this class cannot change its value.

public:
  void setup();
  void switchOn();
  void switchOff();
  void toggle();
  void setIsHeatingOnPointer(std::shared_ptr<const bool> isHeatingOnPointer);
  void autoUpdateStatus();
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
