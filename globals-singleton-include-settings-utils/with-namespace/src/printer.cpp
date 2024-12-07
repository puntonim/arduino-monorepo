#include "printer.h"

#include <Arduino.h>

#include "settings.h"

namespace myproject {

// "Soft" singleton global object defined here,
//  but defined as extern and initialized in printer.h.
Printer printer;

// Global vars intialized here,
//  but defined as extern in printer.h.
// Note: `const` global vars can be initialized directly in the header file
//  and without the need to use `extern`.
char PRINTER_NAME[] = "THE PRINTER!";

void Printer::print() {
  Serial.println((String) "printer.cpp | Year: " + settings::YEAR);
  Serial.println((String) "printer.cpp | Next year: " +
                 settings::settings.NEXT_YEAR);
}

}  // namespace myproject