/*
 * DISPLAY (I2C 16x2) STOPWATCH
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

/* I wrote 2 alternatives.
 *   ALT. 1: using delay()
 *   RESULT: I run it for 25 mins with the task scheduled every 500ms and the stopwatch was 2 secs late compared to my mobile's stopwatch.
 *    Then I run it for 30 mins, scheduled it every 1 sec and it was 3 secs late compared to my mobile's stopwatch.
 *
 *   ALT. 2: using TaskManager | ** BEST ALTERNATIVE **
 *   The code is more elegant, however I want to make sure it is always in sync.
 *   RESULT: I run it for 27 mins with the task scheduled every 500ms and the stopwatch was 2 secs late compared to my mobile's stopwatch.
 *    Then I run it for 25 mins, scheduled it every 1 sec and it was 3 secs late compared to my mobile's stopwatch.
 *
 * CONCLUSIONS:
 *  - The internal clock is not very precise. It' about 4-6 secs late in an hour.
 *  - Use the task manager because is it clean code and it doesn not influence the in ternal clock delay.
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

unsigned long nowMs;

struct Time {
  int h;
  int m;
  int s;
};
void tick(unsigned long delayMs = 0); // Declare it here as it's using a default param.

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
  lcd.print("Stopwatch demo");

  // // ALT. 2: using taskManager.
  // taskManager.scheduleFixedRate(1, [] {
  //   tick();
  // }, TIME_SECONDS); // Or TIME_MILLIS.
}

void loop() {
  // ALT. 1: using delay();
  int delayMs = 1000;
  tick(delayMs);
  
  // ALT. 2: using taskManager.
  // taskManager.runLoop();
}

void tick(unsigned long delayMs) {
  // delayMs defaults to 0.

  unsigned long newNowMs = floor(millis() / 1000);

  if (newNowMs <= nowMs) {
    return;
  }
  nowMs = newNowMs;
  // Serial.println(nowMs);

  struct Time nowTime = _millisToTime(nowMs);
  // Serial.println((String) nowTime.h + ":" + nowTime.m + ":" + nowTime.s);
  char nowString[10]; // 10 cause of the final null appended by spritnf.
                      // Hmmmm I think 9 is enough unless the hours can be 3 digits like
                      //  103 hours.
  sprintf_P(nowString, (PGM_P)F("%02d:%02d:%02d"), nowTime.h, nowTime.m, nowTime.s);  
  lcd.home();
  lcd.print(nowString);

  if (delayMs > 0) {
    delay(delayMs);
  }
}

struct Time _millisToTime(unsigned long ms) {
  // Note: it would have been better to use `std::div` that returns the result and the
  //  reminder performing the division only once.
  int h = floor(ms / 3600);
  unsigned long reminder = ms % 3600;
  int m = floor(reminder / 60);
  int s = reminder % 60;
  struct Time time;
  time.h = h;
  time.m = m;
  time.s = s;
  return time; 
}

