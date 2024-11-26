//********** CLASS DisplayDevice ********************************************************

void DisplayDevice::setup() {
  Wire.begin();
  lcd.begin(16, 2);
  // Most I2C backpacks have the backlight on pin 3.
  lcd.configureBacklightPin(3, LiquidCrystal::BACKLIGHT_NORMAL);
  switchOn();

  pubSub.subscribe([this](ButtonPressEvent* pEvent) {
#if IS_DEBUG == true
    Serial.println((String) "DisplayDevice - received event: " + pEvent->topic);
#endif
    this->switchOn();
  });

  pubSub.subscribe([this](HeatingStatusChangeEvent* pEvent) {
#if IS_DEBUG == true
    Serial.println((String) "DisplayDevice - received event: " + pEvent->topic + " isOn=" + (pEvent->isOn ? "ON" : "OFF"));
#endif
    this->_isHeatingOn = pEvent->isOn;
  });
}

void DisplayDevice::toogle() {
  if (_isDisplayOn == true) switchOff();
  else switchOn();
}

void DisplayDevice::switchOff(bool doResetSwitchOffDisplayTaskId /* = true */) {
  lcd.noBacklight();
  lcd.noDisplay();
  _isDisplayOn = false;

  // We need to reset switchOffDisplayTaskId (or the display will always stay on).
  // No need to cancel any existing tasks to switch off the display because we
  //  have just switched it off, so let those tasks run (but there shouldn't be any).
  if (doResetSwitchOffDisplayTaskId) switchOffDisplayTaskId = TASKMGR_INVALIDID;

    // And cancel any existing tasks to display data because no needed anymore.
#if IS_DEBUG == true
  Serial.println((String) "DisplayDevice - stopping the display task");
#endif
  cancelTask(displayDataTaskId);
  // And reset the counters.
  _counterForDisplayDataExecutions = 0;
  _indexForCurrentlyDisplayedErrorMsg = -1;
}

void DisplayDevice::switchOn(bool doCancelExistingSwitchOffDisplayTask /* = true */) {
  if (doCancelExistingSwitchOffDisplayTask) {
    // Cancel any existing tasks to switch off display because they have an old schedule.
#if IS_DEBUG == true
    Serial.println((String) "DisplayDevice - stopping the switch off task");
#endif
    cancelTask(switchOffDisplayTaskId);
  }
  // And schedule a new task to switch off display.
#if IS_DEBUG == true
  Serial.println((String) "DisplayDevice - starting a new switch off task");
#endif
  switchOffDisplayTaskId = taskManager.schedule(onceSeconds(settings.DISPLAY_SWITCHOFF_TIMER), [] {
    displayDevice.switchOff();
  });

  // If the diplay is already ON, then nothing to do.
  if (!_isDisplayOn) {
    _printData();
    lcd.backlight();
    lcd.display();
    _isDisplayOn = true;
  }
}

/**
 * Display data on a 16x2 screen.
 *
 * It schedules itself to be run every 1 sec or so in order to update the printed data.
 */
void DisplayDevice::_printData() {
  // Display the 2 rows.
  _printFirstRow();
  _printSecondRow();

  // We are done displaying data, so increment the executions counter.
  _counterForDisplayDataExecutions++;

  // Finally schedule a periodic task to update the data shown on display.
  // We cancel this task later on when the display is switched off.
  if (displayDataTaskId == TASKMGR_INVALIDID) {
#if IS_DEBUG == true
    Serial.println((String) "DisplayDevice - starting a new display task");
#endif
    displayDataTaskId = taskManager.scheduleFixedRate(1000, [] {
      displayDevice._printData();
    });
  }
}

void DisplayDevice::_printFirstRow() {
  RowPrinter p(0);

  if (_isHeatingOn == true) p.print("ON ");
  else p.print("OFF ");

  p.print(settings.TARGET_T);
  p.print("\xDF");  // Or: p.print("\xDF""C");
  p.printFillingBlanks();
}

void DisplayDevice::_printSecondRow() {
  RowPrinter p(1);

  // If there are errors,
  //  and this is not the first execution after the button was pressed (when we have to print the regular row)
  //  then we need to show different error msgs every 3 seconds on rotation
  //  (which means that every 3 seconds we print the next error msg in the list).
  if (errorManager.areThereErrors() && (_counterForDisplayDataExecutions > 0)) {
    // Reminder division by 3, and do something only if reminder is 0.
    auto dv = std::div(_counterForDisplayDataExecutions, 3);
    if (dv.rem != 0) return;
    else {
      // It's time to rotate the msg on the display.
      int size = errorManager.getErrorMessageListForDisplay().size();
      // If we have already shown all the error msgs, then it's time to show the regular row.
      if (_indexForCurrentlyDisplayedErrorMsg >= (size - 1)) {
        _indexForCurrentlyDisplayedErrorMsg = -1;
      } else {
        // Print the next msg in the list.
        _indexForCurrentlyDisplayedErrorMsg++;
        char* msg = ListUtils::getByIndex(errorManager.getErrorMessageListForDisplay(), _indexForCurrentlyDisplayedErrorMsg);
        if (msg != nullptr) {
          p.print(msg);
          p.printFillingBlanks();
        } else {
          p.print("IndexError");
          p.printFillingBlanks();
#if IS_DEBUG == true
          Serial.println("IndexError in DisplayDevice::_printSecondRow");
#endif
        }
        return;
      }
    }
  }

  Ds18b20SensorException excDs;
  float t1 = ds18b20Sensor.getData(excDs);
  if (excDs != Ds18b20SensorException::Success) {
    p.print("???-");
  } else {
    // To use only 1 decimal.
    p.print(t1, 1);
    // How to print the degree symbol:
    //  https://forum.arduino.cc/t/solved-how-to-print-the-degree-symbol-extended-ascii/438685
    // lcd.print((String)F("\xDF") + "C");
    p.print("|");
  }

  float sht85Data[2];
  Sht85SensorException excSh;
  sht85Sensor.getData(sht85Data, excSh);

  if (excSh != Sht85SensorException::Success) {
    p.print("???");
  } else {
    p.print(sht85Data[0], 1);
    // lcd.print((String)F("\xDF") + "C ");
    p.print("\xDF ");  // Or: p.print("\xDF""C ");
    // To use no decimals.
    p.print(sht85Data[1], 0);
    // To use 2 decimals.
    // lcd.print(sht85Data[1]);
    // Blank filling the whole row that might contain text from the old error screen.
    p.print("%");
  }
  p.printFillingBlanks();
}


//********** CLASS RowPrinter ***********************************************************

/*
 * Print data on a row.
 *
 * Note that you should first print the 1st row and then the 2nd row (or viceversa).
 * Printing a bit of the 1st row, then the 2nd row and then again a bit of the 1st
 *  row will lead to bugs as the cursor would be in the wrong position. To support
 *  this we should write more code.
 */
RowPrinter::RowPrinter(int i) {
  i = i;
  lcd.setCursor(0, i);
}

/**
 * Print something on the row.
 *
 * Note: this method actually use `lcd.print` which inherits from Arduino's 
 *  class Print: https://docs.arduino.cc/language-reference/en/functions/communication/print/
 * And it's defined twice to support both signatures. An alternative implementation would
 *  be to inherit from Print.
 */
template<typename T>
void RowPrinter::print(T value) {
  size_t len = lcd.print(value);
  _currentSize += len;
}

/**
 * Print something on the row with the extra arg.
 *
 * Note: this method actually use `lcd.print` which inherits from Arduino's 
 *  class Print: https://docs.arduino.cc/language-reference/en/functions/communication/print/
 * And it's defined twice to support both signatures. An alternative implementation would
 *  be to inherit from Print.
 */
template<typename T1, typename T2>
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
  int diff = _MAX_SIZE - _currentSize;
  if (diff > 0)
    for (int i = 0; i < diff; i++) lcd.print(" ");
#endif
}
