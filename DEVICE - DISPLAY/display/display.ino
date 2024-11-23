/*
 * DISPLAY (I2C 16x2) DEMO
 * =======================
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

bool isDisplayOn = false;

void setup() {
  Serial.begin(57600);
  Serial.println("Starting LCD example");
  // For I2C versions.
  Wire.begin();

  Serial.println("Configure LCD");
  lcd.begin(16, 2);
  // Most I2C backpacks have the backlight on pin 3.
  lcd.configureBacklightPin(3, LiquidCrystal::BACKLIGHT_NORMAL);
  lcd.backlight();
  isDisplayOn = true;

  lcd.print("msec");
  lcd.print("      25.2");
  // How to print the degree symbol:
  //  https://forum.arduino.cc/t/solved-how-to-print-the-degree-symbol-extended-ascii/438685
  lcd.print((String)F("\xDF") + "C");

  // Task manager is agreat way to manage tasks: https://tcmenu.github.io/documentation/arduino-libraries/taskmanager-io/
  // ATTENTION: when using the task manager, mind that only 1 task can draw the display.
  //  Never draw to the display in two tasks.
  taskManager.scheduleFixedRate(250, [] {
    lcd.setCursor(0, 1);  // col, row.
    float secondsFraction = millis() / 1000.0F;
    lcd.print(secondsFraction);
  });

  taskManager.scheduleFixedRate(1000, [] {
    toogle_display();
  });
}

void loop() {
  taskManager.runLoop();
}

void toogle_display() {
  int secs = millis() / 1000;
  int reminder = secs % 5;
  if (reminder == 0) {
    // Serial.println((String) secs + " | " + reminder);
    if (isDisplayOn == true) {
      lcd.noBacklight();
      lcd.noDisplay();
    } else {
      lcd.backlight();
      lcd.display();
    }
    isDisplayOn = !isDisplayOn;
  }
}