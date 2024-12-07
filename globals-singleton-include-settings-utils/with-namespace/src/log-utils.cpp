#include <Arduino.h>

#include "settings.h"

// Note: I can declare `myproject` as the default namespace for the vars in this
//  file and avoid to type `myproject::`.
// using namespace myproject;

namespace myproject {
namespace log_utils {

void log() {
  Serial.println((String) "log-utils.cpp | Year: " + myproject::settings::YEAR);
  Serial.println((String) "log-utils.cpp | Next year: " +
                 myproject::settings::settings.NEXT_YEAR);
}

}  // namespace log_utils
}  // namespace myproject