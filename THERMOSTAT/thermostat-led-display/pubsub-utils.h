class BasePubSubEvent {
public:
  constexpr static char* topic = "BASE_EVENT";
private:
};

class ButtonPressEvent : public BasePubSubEvent {
public:
  constexpr static char* topic = "BUTTON_PRESS_EVENT";
};

// class SensorDataEvent : public BasePubSubEvent {
// public:
//   constexpr static char* topic = "SENSOR_DATA_EVENT";
//   const float value;
//   SensorDataEvent(float value)
//     : value(value) {}
// };

class PubSub {

private:
  std::list<std::function<void(ButtonPressEvent*)>> _buttonPressSubCallbacks;
  // std::list<std::function<void(SensorDataEvent*)>> _sensorDataSubCallbacks;

public:
  void publish(ButtonPressEvent* pEvent);
  // void publish(SensorDataEvent* pEvent);
  void subscribe(std::function<void(ButtonPressEvent*)> callback);
  // void subscribe(std::function<void(SensorDataEvent*)> callback);
};

PubSub pubSub;
