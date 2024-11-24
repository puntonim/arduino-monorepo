class BasePubSubEvent {
public:
  constexpr static char* topic = "BASE_EVENT";
private:
};

class ButtonPressEvent : public BasePubSubEvent {
public:
  constexpr static char* topic = "BUTTON_PRESS_EVENT";
};

class HeatingOnEvent : public BasePubSubEvent {
public:
  constexpr static char* topic = "HEATING_ON_EVENT";
};

class HeatingOffEvent : public BasePubSubEvent {
public:
  constexpr static char* topic = "HEATING_OFF_EVENT";
};

class ErrorStateEvent : public BasePubSubEvent {
public:
  constexpr static char* topic = "ERROR_EVENT";
  // TODO add error list as constructor arg
};

class NoErrorStateEvent : public BasePubSubEvent {
public:
  constexpr static char* topic = "NO_ERROR_EVENT";
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
  std::list<std::function<void(HeatingOnEvent*)>> _heatingOnSubCallbacks;
  std::list<std::function<void(HeatingOffEvent*)>> _heatingOffSubCallbacks;
  std::list<std::function<void(ErrorStateEvent*)>> _errorStateSubCallbacks;
  std::list<std::function<void(NoErrorStateEvent*)>> _noErrorStateSubCallbacks;
  // std::list<std::function<void(SensorDataEvent*)>> _sensorDataSubCallbacks;

public:
  void publish(ButtonPressEvent* pEvent);
  void publish(HeatingOnEvent* pEvent);
  void publish(HeatingOffEvent* pEvent);
  void publish(ErrorStateEvent* pEvent);
  void publish(NoErrorStateEvent* pEvent);
  // void publish(SensorDataEvent* pEvent);
  void subscribe(std::function<void(ButtonPressEvent*)> callback);
  void subscribe(std::function<void(HeatingOnEvent*)> callback);
  void subscribe(std::function<void(HeatingOffEvent*)> callback);
  void subscribe(std::function<void(ErrorStateEvent*)> callback);
  void subscribe(std::function<void(NoErrorStateEvent*)> callback);
  // void subscribe(std::function<void(SensorDataEvent*)> callback);
};

PubSub pubSub;
