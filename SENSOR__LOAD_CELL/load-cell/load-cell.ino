/*
 * How to change x and y scale in Serial Plotter: https://www.youtube.com/watch?v=WtOd8tsrJbs
 * In brief: 
 *  - edit the file: /Applications/Arduino\ IDE.app/Contents/Resources/app/lib/backend/resources/arduino-serial-plotter-webapp/static/js/main.35ae02cb.chunk.js
 *  - to have a larger x scale, replace the 50 with 1000 in the string: o.useState)(50)
 *  - to have a fixed y scale, watch the video.
 *
 * Libs:
 *  - HX711 by Rob Tillaart
 */

#include "HX711.h"

// HX711 circuit wiring.
const int LOADCELL_DOUT_PIN = 2;
const int LOADCELL_SCK_PIN = 3;

HX711 scale;

float factor = 1106;


void setup() {
  Serial.begin(57600);
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(factor);
  scale.tare();	
}


void loop() {
  // calibrate();
  // refine_calibration();
  measure_weight();
  // count_time_to_collect_samples(92);
  // delay(1);
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
  Serial.println((String) value + " g - factor=" + factor);

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
