LiquidCrystalI2C_RS_EN(lcd, 0x27, false);

bool isDisplayOn = false;

taskid_t switchOffDisplayTaskId = TASKMGR_INVALIDID;
taskid_t displayDataTaskId = TASKMGR_INVALIDID;

void setupDisplay() {
  Wire.begin();
  lcd.begin(16, 2);
  // Most I2C backpacks have the backlight on pin 3.
  lcd.configureBacklightPin(3, LiquidCrystal::BACKLIGHT_NORMAL);
  switchOnDisplay();
}

void toogleDisplay() {
  if (isDisplayOn == true) switchOffDisplay();
  else switchOnDisplay();
}

void switchOffDisplay(bool doResetSwitchOffDisplayTaskId /* = true */) {
  lcd.noBacklight();
  lcd.noDisplay();
  isDisplayOn = false;

  // We need to reset switchOffDisplayTaskId (or the display will always stay on).
  // No need to cancel any existing tasks to switch off the display because we
  //  have just switched it off, so let those tasks run (but there shouldn't be any).
  if (doResetSwitchOffDisplayTaskId) switchOffDisplayTaskId = TASKMGR_INVALIDID;

  // And cancel any existing tasks to display data because no needed anymore.
  cancelTask(displayDataTaskId);
}

void switchOnDisplay(bool doCancelExistingSwitchOffDisplayTask /* = true */) {
  if (doCancelExistingSwitchOffDisplayTask) {
    // Cancel any existing tasks to switch off display because they have an old schedule.
    cancelTask(switchOffDisplayTaskId);
  }
  // And schedule a new task to switch off display.
  switchOffDisplayTaskId = taskManager.schedule(onceSeconds(10), [] {
    switchOffDisplay();
  });

  // If the diplay is already ON, then nothing to do.
  if (!isDisplayOn) {
    _displayData();
    lcd.backlight();
    lcd.display();
    isDisplayOn = true;
  }
}

/**
 * Display data on a 16x2 screen.
 *
 * Make sure to update the shown data every 1 sec or so.
 */
void _displayData() {
  // Display the 2 rows.
  _displayFirstRow();
  _displaySecondRow();

  // Then schedule a periodic task to update the data shown on display.
  // We cancel this task later on when the display is switched off.
  if (displayDataTaskId == TASKMGR_INVALIDID) {
    displayDataTaskId = taskManager.scheduleFixedRate(1000, [] {
      _displayData();
    });
  }
}

void _displayFirstRow() {
  lcd.setCursor(0, 0);

  float t1 = getDs18b20Data();

  if (t1 == SENSOR_ERROR) {
    lcd.print("NOK");  // TODO blink red led!!
  } else {
    // To use only 1 decimal.
    lcd.print(t1, 1);
    // How to print the degree symbol:
    //  https://forum.arduino.cc/t/solved-how-to-print-the-degree-symbol-extended-ascii/438685
    // lcd.print((String)F("\xDF") + "C");
    lcd.print("\xDF"
              "C");
  }
}

void _displaySecondRow() {
  // noop.
}