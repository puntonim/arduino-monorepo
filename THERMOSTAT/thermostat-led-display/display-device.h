LiquidCrystalI2C_RS_EN(lcd, 0x27, false);

taskid_t switchOffDisplayTaskId = TASKMGR_INVALIDID;
taskid_t displayDataTaskId = TASKMGR_INVALIDID;


class DisplayDevice {

private:
  bool _isDisplayOn = false;
  unsigned int _counterForDisplayDataExecutions = 0;
  int _indexForCurrentlyDisplayedErrorMsg = -1;
  void _printData();
  void _printFirstRow();
  void _printSecondRow();

  // Data on display.
  bool _isHeatingOn = false;

public:
  void setup();
  void toogle();
  void switchOff(bool doResetSwitchOffDisplayTaskId = true);
  void switchOn(bool doCancelExistingSwitchOffDisplayTask = true);
  void setIsHeatingOnPointer(std::shared_ptr<const bool> isHeatingOnPointer);
};


class RowPrinter {

private:
  const unsigned int _MAX_SIZE = 16;
  unsigned int _currentSize = 0;

public:
  RowPrinter(int i);
  int i;
  template <typename T>
  void print(T value);
  template <typename T1, typename T2>
  void print(T1 value, T2 extra);
  void printFillingBlanks();
};
