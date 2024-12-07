/**
 * TLDR: I RECOMMEND THE VERSION THAT USES NAMESPACE.
 *
 * Global vars and "soft" singletons.
 * Also, settings  and utils.
 * Utils uses namespace.
 *
 * Disadvantages versus the version using namespaces:
 *  - global vars are discouraged because they can be accidentally used by
 *     external libraries. Eg. the `logging` or `logging::logger` object;
 *    by using namespaces we limit their scope;
 *
 * Advantages versus the version using namespaces:
 *  - simpler;
 *
 * Note: "soft" singleton is the typical Python pattern where the client reuse
 *  the same global var as singleton. It's only "soft" because the client can
 *  actually instantiate a new object.
 */
#include <Arduino.h>

#include "log-utils.h"
#include "printer.h"
#include "settings.h"

void setup() {
#if IS_DEBUG == true
  Serial.begin(57600);
  unsigned short i = 30;
  while (!Serial && i--) delay(1);
  Serial.println("START");
#endif
}

void loop() {
  // Settings as global vars.
  Serial.println((String) "main.cpp | Year: " + YEAR);
  // **BEST**  Settings in a "soft" singleton global object.
  Serial.println((String) "main.cpp | Next year: " + settings.NEXT_YEAR);

  // Utils functions in namespace.
  log_utils::log();

  // "Soft" singleton global object.
  printer.print();
  // Global var.
  Serial.println((String) "main.cpp | Printer name: " + PRINTER_NAME);

  Serial.println();
  delay(1000);
}
