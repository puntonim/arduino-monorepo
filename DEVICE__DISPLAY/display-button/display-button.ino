/*
 * DISPLAY (I2C 16x2) WITH BUTTON
 * ==============================
 * Display the seconds since boot.
 * The display switches off (standby) after 10 secs.
 * Push the button to activate the display.
 *
 * Note: I bought 2 displays for 9 EUR on Amazon.it.
 * They have a I2C backpack, so the are only 4 wires to connect.
 * 
 * Display:
 *   1 row: seconds      TEST
 *   2 row: [# seconds since boot]
 * Button: switch on the display.
 * Note: after pressing the button 1 non-repeating task is scheduled with TaskManagerIO and lives forever.
 *
 * Devices:
 *  1x push button
 *  1x display I2C 16x2 
 * The display switches off automatically after 10 seconds.
 *
 * Circuit for display:
 *  I2C > Arduino Uno R4
 *  ---   --------------
 *  GND > GND
 *  VCC > 5V
 *  SDA > SDA
 *  SCL > SCL
 *
 * Libs:
 *  - LiquidCrystalIO by Arduino, Adafruit, theCodersCorner https://docs.arduino.cc/libraries/liquidcrystal/
 *  - TaskManagerIO by davetcc https://github.com/TcMenu/TaskManagerIO
 */

/* I evaluated 2 libs to drive the display.
 * 1) LiquidCrystalIO by Arduino, Adafruit, theCondersConern - THE BEST
 * https://www.arduino.cc/reference/en/libraries/liquidcrystalio/
 * https://github.com/TcMenu/LiquidCrystalIO/
 * 2) HD44780 by Bill Perry
 * https://www.arduino.cc/reference/en/libraries/hd44780/
 * https://github.com/duinoWitchery/hd44780/tree/master/examples/ioClass/hd44780_I2Cexp
 */


#include <LiquidCrystalIO.h>
#include <IoAbstractionWire.h>

// Declaration of functions with default args.
void switchOnDisplay(bool doCancelExistingSwitchOffDisplayTask = true);
void switchOffDisplay(bool doResetSwitchOffDisplayTaskId = true);

void setup() {
  Serial.begin(57600);

  Serial.println("Starting LCD example");

  setupButton();
  setupDisplay();
}

void loop() {
  // TODO disable this debug.
  // printDebugTasks();

  taskManager.runLoop();
}
