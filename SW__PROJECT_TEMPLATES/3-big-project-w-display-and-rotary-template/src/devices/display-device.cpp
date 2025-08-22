#include "devices/display-device.h"

#include <LiquidCrystalIO.h>
// IoAbstractionWire.h required when using LiquidCrystalIO I2C version.
#include <IoAbstractionWire.h>

#include "domain/main-domain.h"
#include "utils/pubsub-utils.h"
#include "utils/task-manager-utils.h"

// This cannot be inside the namespace, not sure why.
LiquidCrystalI2C_RS_EN(lcd, 0x27, false);

namespace myproject {

//
//********** CLASS DisplayDevice ***********************************************

// "Soft" singleton global object defined here,
//  but defined as extern and initialized in led-device.h.
DisplayDevice displayDevice;

void DisplayDevice::setup() {
  Wire.begin();
  lcd.begin(16, 2);
  // Most I2C backpacks have the backlight on pin 3.
  lcd.configureBacklightPin(3, LiquidCrystal::BACKLIGHT_NORMAL);
  switchOn();

  // SUBSCRIPTION TimerRotaryPressEvent --------------------------------------
  pubsub_utils::pubSub.subscribe(
      [this](pubsub_utils::TimerRotaryPressEvent* pEvent) {
#if IS_DEBUG == true
        Serial.println((String) "DisplayDevice - received event: " +
                       pEvent->topic);
#endif
        // Switch ON the display if not already ON, cancel and reschedule
        //  the switch OFF task in 20 sec.
        this->switchOn();
      });

  // SUBSCRIPTION TimerRotaryRotationEvent -----------------------------------
  pubsub_utils::pubSub.subscribe(
      [this](pubsub_utils::TimerRotaryRotationEvent* pEvent) {
#if IS_DEBUG == true
        Serial.println((String) "DisplayDevice - received event: " +
                       pEvent->topic);
#endif
        // Switch ON the display if not already ON, cancel and reschedule
        //  the switch OFF task in 20 sec.
        this->switchOn();
      });

  // SUBSCRIPTION TimerStartEvent ------------------------------------------
  pubsub_utils::pubSub.subscribe([this](pubsub_utils::TimerStartEvent* pEvent) {
#if IS_DEBUG == true
    Serial.println((String) "DisplayDevice - received event: " + pEvent->topic);
#endif
    this->_refreshFirstRow();
  });

  // SUBSCRIPTION TimerEndEvent --------------------------------------------
  pubsub_utils::pubSub.subscribe([this](pubsub_utils::TimerEndEvent* pEvent) {
#if IS_DEBUG == true
    Serial.println((String) "DisplayDevice - received event: " + pEvent->topic);
#endif
    if (_isOn) this->_refreshFirstRow();
  });

  // SUBSCRIPTION TimerUpdateEvent ------------------------------------
  pubsub_utils::pubSub.subscribe([this](
                                     pubsub_utils::TimerUpdateEvent* pEvent) {
#if IS_DEBUG == true
    Serial.println((String) "DisplayDevice - received event: " + pEvent->topic);
#endif
    this->_refreshFirstRow();
  });
}

void DisplayDevice::toogle() {
  if (_isOn == true)
    switchOff();
  else
    switchOn();
}

bool DisplayDevice::isOn() { return _isOn; }

void DisplayDevice::switchOff(bool doResetSwitchOffDisplayTaskId /* = true */) {
  lcd.noBacklight();
  lcd.noDisplay();
  _isOn = false;

  // We need to reset switchOffDisplayTaskId (or the display will always stay
  //  on). No need to cancel any existing tasks to switch off the display
  //  because we have just switched it off, so let those tasks run (but there
  //  shouldn't be any).
  if (doResetSwitchOffDisplayTaskId) switchOffTaskId = TASKMGR_INVALIDID;

  // And cancel any existing tasks to display data because no needed anymore.
#if IS_DEBUG == true
  Serial.println((String) "DisplayDevice - stopping the display task #" +
                 printRowsTaskId);
#endif
  task_manager_utils::cancelTask(printRowsTaskId);
  // And reset the counters.
  // Counter used to scroll error messages.
  _counterForPrintRowsExecutions = 0;
  _indexForCurrentlyDisplayedErrorMsg = -1;
}

void DisplayDevice::switchOn(
    bool doCancelExistingSwitchOffDisplayTask /* = true */) {
  if (doCancelExistingSwitchOffDisplayTask) {
    // Cancel any existing tasks to switch off display because they have an old
    // schedule.
#if IS_DEBUG == true
    Serial.println((String) "DisplayDevice - stopping the switch off task #" +
                   switchOffTaskId);
#endif
    task_manager_utils::cancelTask(switchOffTaskId);
  }

  // Schedule a new task to switch off display.
#if IS_DEBUG == true
  Serial.println((
      String) "DisplayDevice - starting a new switch off "
              "task");
#endif
  switchOffTaskId =
      taskManager.schedule(onceSeconds(settings::DISPLAY_SWITCHOFF_TIMER),
                           [] { displayDevice.switchOff(); });

  // If the display is already ON, then nothing to do.
  if (!_isOn) {
    _isOn = true;
    _printRows();
    lcd.backlight();
    lcd.display();
  }
}

/**
 * Display data on a 16x2 screen.
 *
 * It schedules itself to be run every 1 sec or so in order to update the
 * printed data.
 */
void DisplayDevice::_printRows() {
  // Display the 2 rows.
  _printFirstRow();
  _printSecondRow();

  // We are done printing rows, so increment the executions counter (used to
  //  scroll error messages).
  _counterForPrintRowsExecutions++;

  // Finally schedule a periodic task to update the data shown on display.
  // We cancel this task later on when the display is switched off.
  if ((printRowsTaskId == TASKMGR_INVALIDID) && _isOn) {
#if IS_DEBUG == true
    Serial.println(
        "DisplayDevice::_printRows - starting a new "
        "display task");
#endif
    printRowsTaskId = taskManager.schedule(repeatSeconds(1),
                                           [] { displayDevice._printRows(); });
  }
}

void DisplayDevice::_printFirstRow() {
  RowPrinter p(0);

  mainDomain.tick();

  if (mainDomain.timer.isOver()) {
    p.print("OFF");
    p.printFillingBlanks();
    return;
  }

  // Format time like: 1:04:09
  // Size 9 because of the final null appended by spritnf. And the hour can be
  //  2 digits (eg, "26" hours) even if we don't do the 0-filling (so 1 hour
  //  is printed as "1" and not "01").
  char timerTimeString[9];
  // sprintf_P(timerTimeString, (PGM_P)F("%1d:%02d:%02d"), timerTime.h,
  // timerTime.m, timerTime.s);
  mainDomain.timer.format(timerTimeString);
  p.print(timerTimeString);

  p.printFillingBlanks();
}

void DisplayDevice::_printSecondRow() {
  RowPrinter p(1);
  p.print("Turn the rotary");
  p.printFillingBlanks();
}

void DisplayDevice::_refreshFirstRow() {
  if (!_isOn) return switchOn();
  task_manager_utils::cancelTask(printRowsTaskId);
  _printFirstRow();
  if ((printRowsTaskId == TASKMGR_INVALIDID) && _isOn) {
#if IS_DEBUG == true
    Serial.println(
        "DisplayDevice::_refreshFirstRow - starting a "
        "new display task");
#endif
    printRowsTaskId = taskManager.schedule(repeatSeconds(1),
                                           [] { displayDevice._printRows(); });
  }
}

//
//********** CLASS RowPrinter **************************************************

/*
 * Print data on a row.
 *
 * Note that you should first print the 1st row and then the 2nd row (or
 * viceversa). Printing a bit of the 1st row, then the 2nd row and then again a
 * bit of the 1st row will lead to bugs as the cursor would be in the wrong
 * position. To support this we should write more code.
 */
RowPrinter::RowPrinter(unsigned short i) {
  i = i;
  lcd.setCursor(0, i);
}

/**
 * Print something on the row.
 *
 * Note: this method actually use `lcd.print` which inherits from Arduino's
 *  class Print:
 * https://docs.arduino.cc/language-reference/en/functions/communication/print/
 * And it's defined twice to support both signatures. An alternative
 * implementation would be to inherit from Print.
 */
template <typename T>
void RowPrinter::print(T value) {
  size_t len = lcd.print(value);
  _currentSize += len;
}

/**
 * Print something on the row with the extra arg.
 *
 * Note: this method actually use `lcd.print` which inherits from Arduino's
 *  class Print:
 * https://docs.arduino.cc/language-reference/en/functions/communication/print/
 * And it's defined twice to support both signatures. An alternative
 * implementation would be to inherit from Print.
 */
template <typename T1, typename T2>
void RowPrinter::print(T1 value, T2 extra) {
  size_t len = lcd.print(value, extra);
  _currentSize += len;
}

/**
 * Fill the row with blanks.
 *
 * The purpose is to overwrite old text, longer than the one we are displaying.
 * Howerver this can hide some bugs (concurrent prints) so we might disable it
 *  on dev by setting DO_ENABLE_FILLING_BLANKS = false.
 */
void RowPrinter::printFillingBlanks() {
#if DO_ENABLE_FILLING_BLANKS == true
  short diff = _MAX_SIZE - _currentSize;
  for (int i = 0; i < diff; i++) lcd.print(" ");
  _currentSize = _MAX_SIZE;
#endif
}

/**
 * Print the given string to the rightmost part of the row.
 *
 * Note: it would be better to make this function generic (like print()) but
 *  then it's harder to compute the size of the string, as the string is the
 *  conversion of a generic type to string.
 */
void RowPrinter::printRightAlign(char* string) {
  short diff = _MAX_SIZE - _currentSize - strlen(string);
  for (int i = 0; i < diff; i++) lcd.print(" ");
  size_t len = lcd.print(string);
  _currentSize += len;
}

}  // namespace myproject
