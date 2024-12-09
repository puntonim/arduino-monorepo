#include "utils/time-utils.h"

#include <Arduino.h>

namespace tstat {
namespace time_utils {

unsigned long timeToSecs(Time time) {
  return time.s + (time.m * 60) + (time.h * 3600);
}

Time secsToTime(unsigned long secs) {
  Time t;
  auto dv = div(secs, 60);
  t.s = dv.rem;
  dv = div(dv.quot, 60);
  t.m = dv.rem;
  t.h = dv.quot;
  return t;
}

void Timer::start(unsigned short hour /*  = settings::DEFAULT_TIMER.h */,
                  unsigned short minute /*  = settings::DEFAULT_TIMER.m */,
                  unsigned short second /*  = settings::DEFAULT_TIMER.s */) {
  _time.h = hour;
  _time.m = minute;
  _time.s = second;
  _lastTickTs = millis();
}

Time Timer::addTime(unsigned short hour, unsigned short minute,
                    unsigned short second) {
  _time.h += hour;
  _time.m += minute;
  _time.s += second;
  return _time;
}

Time Timer::tick() {
  auto nowTs = millis();
  // Elapsed secs since last tick.
  // round(10/3.); // 3.33 -> 3
  // round(11/3.); // 3.66 -> 4
  int elapsedSecs = (int)round((nowTs - _lastTickTs) / 1000.0);  // Secs (int).
  if (elapsedSecs < 0) {
    // Note: millis() resets to 0 after ~49 days, precisely after it reaches the
    // max value for the type unsigned long.
    // https://docs.arduino.cc/language-reference/en/functions/time/millis/
    elapsedSecs = (int)round(
        (std::numeric_limits<unsigned long>::max() - _lastTickTs + nowTs) /
        1000.0);
  }
  if (elapsedSecs >= 1) {
    _lastTickTs = nowTs;
    int newTimeInSecs = timeToSecs(_time) - elapsedSecs;
    if (newTimeInSecs < 0) newTimeInSecs = 0;
    _time = secsToTime(newTimeInSecs);
  }
  return _time;
}

void Timer::format(char* string) { time_utils::format(string, _time); }

bool Timer::isOver() { return time_utils::isOver(_time); }

void Timer::reset() {
  _time.h = 0;
  _time.m = 0;
  _time.s = 0;
}

/**
 * Format a Time object to a string like: "1:04:09".
 *
 * Mind that you nedd to provide a string of size 9, as first arg, because of
 * the final NULL appended by spritnf. And the hour can be 2 digits (eg, "26"
 * hours) even if we don't do the 0-filling (so 1 hour is printed as "1" and not
 * "01").
 *
 * Example:
 *   time_utils::Timer timer;
 *   timer.start(1, 2, 11);
 *   ...
 *   time_utils::Time timerTime = timer.tick();
 *   char timerTimeString[9];
 *   time_utils::format(timerTimeString, timerTime);
 *   Serial.println(timerTimeString);
 */
void format(char* string, Time time) {
  sprintf_P(string, (PGM_P)F("%1d:%02d:%02d"), time.h, time.m, time.s);
}

bool isOver(Time time) {
  if ((time.h <= 0) && (time.m <= 0) && (time.s <= 0))
    return true;
  else
    return false;
}

}  // namespace time_utils
}  // namespace tstat
