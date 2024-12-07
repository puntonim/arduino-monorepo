/**
 * TLDR: I RECOMMEND THIS VERSION THAT USES NAMESPACE.
 *
 * Global vars and "soft" singletons in namespaces.
 * Also, utils and settings in nested-namespaces.
 *
 * Advantages versus the version that does NOT use namespaces:
 *  - global vars are discouraged because they can be accidentally used by
 *     external libraries. Eg. the `logging` or `logging::logger` object;
 *    by using namespaces we limit their scope;
 *  - we don't need a `settings.cpp` file to manage settings;
 *
 * Disadvantages versus the version that does NOT use namespaces:
 *  - a bit more complex due to th euse of namespaces;
 *
 * Note: "soft" singleton is the typical Python pattern where the client reuse
 *  the same global var as singleton. It's only "soft" because the client can
 *  actually instantiate a new object.
 */
#include <Arduino.h>

#include "log-utils.h"
#include "printer.h"
#include "settings.h"

// Note: I can declare `myproject` as the default namespace for the vars in this
//  file and avoid to type `myproject::`.
// using namespace myproject;

void setup() {
#if IS_DEBUG == true
  Serial.begin(57600);
  unsigned short i = 30;
  while (!Serial && i--) delay(1);
  Serial.println("START");
#endif
}

void loop() {
  // **BEST** Settings as global vars in nested-namespace.
  Serial.println((String) "main.cpp | Year: " + myproject::settings::YEAR);
  // Settings in a "soft" singleton global object in nested-namespace.
  Serial.println((String) "main.cpp | Next year: " +
                 myproject::settings::settings.NEXT_YEAR);

  // Utils functions in nested-namespace.
  myproject::log_utils::log();

  // "Soft" singleton global object in project-namespace.
  myproject::printer.print();
  // Global var in project-namespace.
  Serial.println((String) "main.cpp | Printer name: " +
                 myproject::PRINTER_NAME);

  Serial.println();
  delay(1000);
}
