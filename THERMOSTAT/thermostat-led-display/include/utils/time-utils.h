#ifndef _TIME_UTILS_H
#define _TIME_UTILS_H

namespace tstat {
namespace time_utils {

struct Time {
  short h;
  short m;
  short s;
};

unsigned long timeToSecs(Time time);
Time secsToTime(unsigned long secs);
void format(char* string, Time time);
bool isOver(Time time);

class Timer {
 private:
  Time _time;
  unsigned long _lastTickTs;

 public:
  void start(unsigned short hour, unsigned short minute, unsigned short second);
  Time addTime(unsigned short hour, unsigned short minute,
               unsigned short second);
  Time tick();
  void format(char* string);
  bool isOver();
};

}  // namespace time_utils
}  // namespace tstat
#endif
