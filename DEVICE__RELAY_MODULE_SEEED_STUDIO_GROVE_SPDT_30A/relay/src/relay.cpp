/*
 * RELAY MODULE SEEED STUDIO GROVE SPDT 30A
 * ========================================
 * Control a relay module. A relay is a switch that is controlled electronically
 *  rather than manually (like a wall switch that we trigger with our fingers
 *  to switch on/off a bulb).
 * Seeed Studio Grove SPDT 30A is a relay module for high voltage AC that
 *  supports a load of 220V and 30A AC. It can be controlled by Arduino with
 *  5V DC.
 * The actual relay is a Songle SLA-05VDC-SL-C.
 * https://www.seeedstudio.com/Grove-SPDT-Relay-30A.html
 *
 * SPDT relays have 3 outputs:
 *  - COM: one side of the connection to be switched
 *  - NO (normally open): when the relay is not triggered (by Arduino's signal)
 * this wire is NOT connected to COM. It gets connected to COM when the realy is
 * ON.
 *  - NC (normnally closed): when the relay is not triggered (by Arduino's
 * signal) this wire is connected to COM. It gets disconnected to COM when the
 * realy is ON.
 * IMP: USE EITHER NO OR NC!!!!!!!!
 *
 * This example switches on/off the relay (there is a LED integrated in the
 * module) every 5 seconds.
 *
 * Devices:
 *  1x relay Seeed Studio Grove SPDT 30A
 */

#include <Arduino.h>

const u_int8_t RELAY_S_PIN = 2;

void setup() { pinMode(RELAY_S_PIN, OUTPUT); }

void loop() {
  delay(5000);
  digitalWrite(RELAY_S_PIN, HIGH);
  delay(5000);
  digitalWrite(RELAY_S_PIN, LOW);
}
