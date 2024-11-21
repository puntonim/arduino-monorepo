/*
 * LED MATRIX DEMO
 * ===============
 * Blink the whole onboard led matrix.
 *
 * Docs: https://docs.arduino.cc/tutorials/uno-r4-wifi/led-matrix/
 */

#include "Arduino_LED_Matrix.h"

ArduinoLEDMatrix matrix;

// uint8_t frame[8][12] = {
//   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
// };

uint8_t frame[8][12] = {
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

void on() {
  for (int r = 0; r<8; r++) {
    for (int c=0; c<12; c++) {
      frame[r][c] = 1;
    }
  }
}

void off() {
  for (int r = 0; r<8; r++) {
    for (int c=0; c<12; c++) {
      frame[r][c] = 0;
    }
  }
}

void setup() {
  Serial.begin(57600);
  matrix.begin();
}

void loop() {
  on();
  matrix.renderBitmap(frame, 8, 12);
  delay(100);
  off();
  matrix.renderBitmap(frame, 8, 12);
  delay(100);
}
