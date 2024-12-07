#ifndef _SETTINGS_H
#define _SETTINGS_H

#define IS_DEBUG true

namespace myproject {
namespace settings {

// NOTE: IT DOESN'T MAKE MUCH SENSE TO USE A `struct` HERE,
//  JUST USE REGULAR VARS FOR SETTINGS.
// "Soft" singleton global object defined as `const` and initialized here,
//  but also defined in settings.cpp.
struct Settings {
  int NEXT_YEAR = 2025;
};
// `extern` not needed because it's `const`.
const Settings settings;

// Global vars defined as `const` here,
//  but intialized in settings.cpp.
// `extern` not needed because it's `const`.
const int YEAR = 2024;

}  // namespace settings
}  // namespace myproject

#endif
