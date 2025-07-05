#ifndef _PUBSUB_UTILS_H
#define _PUBSUB_UTILS_H

#include <functional>
#include <list>

namespace bigpjtemplate {
namespace pubsub_utils {

class BasePubSubEvent {
 public:
  constexpr static char topic[] = "BASE_EVENT";

 private:
};

class TimerStartEvent : public BasePubSubEvent {
 public:
  constexpr static char topic[] = "TIMER_START_EVENT";
};

class TimerFinishEvent : public BasePubSubEvent {
 public:
  constexpr static char topic[] = "TIMER_FINISH_EVENT";
  unsigned long tsMillis;
  TimerFinishEvent(unsigned long tsMillis) : tsMillis(tsMillis) {};
};

class PubSub {
 private:
  std::list<std::function<void(TimerStartEvent*)>> _timerStartSubCallbacks;
  std::list<std::function<void(TimerFinishEvent*)>> _timerFinishSubCallbacks;

 public:
  void publish(TimerStartEvent* pEvent);
  void publish(TimerFinishEvent* pEvent);

  void subscribe(std::function<void(TimerStartEvent*)> callback);
  void subscribe(std::function<void(TimerFinishEvent*)> callback);
};

// "Soft" singleton global object defined as extern and initialized here,
//  but also defined in pubsub-utils.cpp.
extern PubSub pubSub;

}  // namespace pubsub_utils
}  // namespace bigpjtemplate
#endif
