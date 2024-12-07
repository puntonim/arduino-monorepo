#include <Arduino.h>

#include "settings.h"

namespace log_utils {

void log() {
  Serial.println((String) "log-utils.cpp | Year: " + YEAR);
  Serial.println((String) "log-utils.cpp | Next year: " + settings.NEXT_YEAR);
}

}  // namespace log_utils