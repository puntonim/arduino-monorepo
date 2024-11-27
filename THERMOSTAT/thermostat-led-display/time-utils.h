namespace time_utils
{

  struct Time {
    short h;
    short m;
    short s;
  };

  unsigned long timeToSecs(struct Time time);
  struct Time secsToTime(unsigned long secs);
  void format(char* string, Time time);
  bool isOver(Time time);

  class Timer {
  private:
    struct Time _time;
    unsigned long _lastTickTs;

  public:
    void start(unsigned short hour, unsigned short minute, unsigned short second);
    struct Time tick();
    void format(char* string);
    bool isOver();
  };
}