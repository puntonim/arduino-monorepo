/*
 * How to change x and y scale in Serial Plotter: https://www.youtube.com/watch?v=WtOd8tsrJbs
 * In brief: 
 *  - edit the file: /Applications/Arduino\ IDE.app/Contents/Resources/app/lib/backend/resources/arduino-serial-plotter-webapp/static/js/main.35ae02cb.chunk.js
 *  - to have a larger x scale, replace the 50 with 1000 in the string: o.useState)(50)
 *  - to have a fixed y scale, watch the video.
 *
 * Libs:
 *  - HX711 by Rob Tillaart
 *  - LiquidCrystalIO by Arduino, Adafruit, theCondersConern 
 */

// Scale libs.
#include <HX711.h>
// Display libs.
#include <Arduino.h>
#include <LiquidCrystalIO.h>
#include <IoAbstractionWire.h>
#include <Wire.h>

// HX711 circuit wiring.
const int LOADCELL_DOUT_PIN = 2;
const int LOADCELL_SCK_PIN = 3;

// Scale object.
HX711 scale;
float factor = 1106;

// DIsplay object.
LiquidCrystalI2C_RS_EN(lcd, 0x27, false);

struct round_value {
  int integer;
  int decimal;
};

void setup() {
  Serial.begin(57600);

  _setup_scale();
  _setup_display();

  // taskManager.scheduleFixedRate(1, [] {
  //   // count_time_to_collect_samples(92);
  //   measure_weight();
  // });

  taskManager.scheduleFixedRate(100, [] {
    measure_weigh_and_display();
  });
}

void _setup_scale() {
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(factor);
  scale.tare();
}

void _setup_display() {
  Wire.begin();
  lcd.begin(16, 2);
  // Most backpacks have the backlight on pin 3.
  lcd.configureBacklightPin(3, LiquidCrystal::BACKLIGHT_NORMAL);
  // Turn on the backlight.
  lcd.backlight();
  lcd.print("Ready");
}


void loop() {
  // calibrate();
  // refine_calibration();
  // measure_weight();
  // count_time_to_collect_samples(92);
  // delay(1);

  taskManager.runLoop();
}


void calibrate() {
  scale.set_scale();
  scale.tare();
  Serial.println("Appoggia il peso campione di 217 g ");
  while (!Serial.available()) {}

  long value = scale.get_units(10);
  long actual_weight = 217;
  long factor = value / actual_weight;

  Serial.println((String) "Factor: " + factor);
  while (1) {}
}


void refine_calibration() {
  scale.set_scale(factor);
  long value = scale.get_units(1);
  if (value < 0) {
    value = 0;
  }
  Serial.println((String)value + " g - factor=" + factor);

  if (Serial.available()) {
    char input = Serial.read();
    if (input == 'a') {
      factor += 3;
    } else if (input == 'z') {
      factor -= 3;
    }
  }
}


void measure_weight() {
  // if (scale.is_ready()) {
  if (scale.wait_ready_timeout(200)) {
    //long value = scale.read();
    long value = scale.get_units(1);
    Serial.println(value);
  } else {
    // Serial.println("HX711 not found.");
  }
}


void count_time_to_collect_samples(int n_samples) {
  if (scale.wait_ready_timeout(200)) {
    unsigned long start_ts = millis();
    for (int i = 0; i < n_samples; i++) {
      long value = scale.get_units(1);
    }
    unsigned long end_ts = millis();
    unsigned long elapsed_sec = end_ts - start_ts;
    Serial.println((String) "#" + n_samples + " samples in ms: " + elapsed_sec);
  }
  delay(1000);
}

void measure_weigh_and_display() {
  // if (scale.is_ready()) {
  if (scale.wait_ready_timeout(200)) {
    //float value = scale.read();
    float value = scale.get_units(20);
    Serial.println(value);

    struct round_value v = _round(value, 1);
    lcd.clear();
    lcd.print(v.integer);
    lcd.print(".");
    lcd.print(v.decimal);
    lcd.print("g");
  } else {
    // Serial.println("HX711 not found.");
  }
}

struct round_value _round(float f, int places) {
  float n = std::pow(10.0f, places);
  float rounded = std::round(f * n) / n;

  float rounded_abs = abs(rounded);
  int integer = floor(rounded_abs);
  if (rounded < 0.0) {
    integer *= -1;
  }

  int x = round(rounded_abs * 10.0);
  int decimal = x % 10;

  struct round_value v;
  v.integer = integer;
  v.decimal = decimal;

  return v;
}
