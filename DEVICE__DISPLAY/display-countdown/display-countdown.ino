/*
 * DISPLAY (I2C 16x2) COUNTDOWN
 * ============================
 * I bought 2 displays for 9 EUR on Amazon.it.
 * They have a I2C backpack, so the are only 4 wires to connect.
 * 
 * Circuit:
 *  I2C > Arduino Uno R4
 *  ---   --------------
 *  GND > GND
 *  VCC > 5V
 *  SDA > SDA
 *  SCL > SCL
 *
 * Libs:
 *  - LiquidCrystalIO by Arduino, Adafruit, theCondersConern
 */

/* I evaluated 2 libs to drive the display.
 * 1) LiquidCrystalIO by Arduino, Adafruit, theCondersConern - THE BEST
 * https://www.arduino.cc/reference/en/libraries/liquidcrystalio/
 * https://github.com/TcMenu/LiquidCrystalIO/
 * 2) HD44780 by Bill Perry
 * https://www.arduino.cc/reference/en/libraries/hd44780/
 * https://github.com/duinoWitchery/hd44780/tree/master/examples/ioClass/hd44780_I2Cexp
 */


#include <Arduino.h>
#include <LiquidCrystalIO.h>
// Required when using the I2C version.
#include <IoAbstractionWire.h>
#include <Wire.h>

// For most standard I2C backpacks one of the two helper functions below will create you a liquid
//  crystal instance that's ready configured for I2C.
// Important Note: this method assumes a PCF8574 running at 100Khz. If otherwise
// use a custom configuration as you see in many other examples.
// If your backpack is wired RS,RW,EN then use this version.
LiquidCrystalI2C_RS_EN(lcd, 0x27, false);
// If your backpack is wired EN,RW,RS then use this version instead of the above.
// LiquidCrystalI2C_EN_RS(lcd, 0x27, false)

const int MAX_INT = 59;
int countdownH = 1;
int countdownM = 12;
int countdownS = 27;
uint8_t countdownTaskId;
uint8_t toogleDisplayTaskId;

void setup() {
  Serial.begin(57600);
  // For I2C versions.
  Wire.begin();

  Serial.println("Configuring LCD");
  lcd.begin(16, 2);
  // Most I2C backpacks have the backlight on pin 3.
  lcd.configureBacklightPin(3, LiquidCrystal::BACKLIGHT_NORMAL);
  lcd.backlight();
  // isDisplayOn = true;

  lcd.print("            21");
  // How to print the degree symbol:
  //  https://forum.arduino.cc/t/solved-how-to-print-the-degree-symbol-extended-ascii/438685
  lcd.print(F("\xDF" "C"));

  lcd.setCursor(0, 1); // col, row.
  lcd.print("Countdown demo");

  // Task manager is a great way to manage tasks: https://tcmenu.github.io/documentation/arduino-libraries/taskmanager-io/
  // ATTENTION: when using the task manager, mind that only 1 task can draw the display.
  //  Never draw to the display in two tasks.
  countdownTaskId = taskManager.scheduleFixedRate(1, &tick1, TIME_SECONDS);
  toogleDisplayTaskId = taskManager.scheduleFixedRate(1, &toogleDisplay, TIME_SECONDS);
}

void loop() {
  taskManager.runLoop();
}

void tick1() {
  countdownS--;
  if (countdownS == -1) {

    if (countdownH == 0 && countdownM == 0) {
      taskManager.cancelTask(countdownTaskId);
      lcd.home();
      lcd.print("OVER     ");
      return;
    }
    countdownM--;
    countdownS = MAX_INT;
    if (countdownM == -1) {
      countdownH--;
      countdownM = MAX_INT;
    }
  }

  char countdownString[10]; // 10 cause of the final null appended by spritnf.
  sprintf_P(countdownString, (PGM_P)F("-%02d:%02d:%02d"), countdownH, countdownM, countdownS);
  
  lcd.home();
  lcd.print(countdownString);
}

void toogleDisplay() {
  if (millis() < 30*1000) {
    return;
  }

  if (countdownH == 0 && countdownM <= 1) {
      lcd.backlight();
      lcd.display();   
      taskManager.cancelTask(toogleDisplayTaskId);
      return;
  }

  if (countdownS <= 5) {
      lcd.backlight();
      lcd.display();
  } else {
      lcd.noBacklight();
      lcd.noDisplay();
  }
}