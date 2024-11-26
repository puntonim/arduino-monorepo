class BasePubSubEvent {
public:
  constexpr static char* topic = "BASE_EVENT";
private:
};

class ButtonPressEvent : public BasePubSubEvent {
public:
  constexpr static char* topic = "BUTTON_PRESS_EVENT";
};

class HeatingStatusChangeEvent : public BasePubSubEvent {
public:
  constexpr static char* topic = "HEATING_STATUS_CHANGE_EVENT";
  const bool isOn;
  HeatingStatusChangeEvent(bool isOn): isOn(isOn) {};
};

class ErrorStateEvent : public BasePubSubEvent {
public:
  constexpr static char* topic = "ERROR_EVENT";
  std::list<char*> messageList;
  ErrorStateEvent(std::list<char*> messageList) : messageList(messageList) {};
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
  std::list<std::function<void(HeatingStatusChangeEvent*)>> _heatingStatusChangeSubCallbacks;
  std::list<std::function<void(ErrorStateEvent*)>> _errorStateSubCallbacks;
  std::list<std::function<void(NoErrorStateEvent*)>> _noErrorStateSubCallbacks;
  // std::list<std::function<void(SensorDataEvent*)>> _sensorDataSubCallbacks;

public:
  void publish(ButtonPressEvent* pEvent);
  void publish(HeatingStatusChangeEvent* pEvent);
  void publish(ErrorStateEvent* pEvent);
  void publish(NoErrorStateEvent* pEvent);
  // void publish(SensorDataEvent* pEvent);
  
  void subscribe(std::function<void(ButtonPressEvent*)> callback);
  void subscribe(std::function<void(HeatingStatusChangeEvent*)> callback);
  void subscribe(std::function<void(ErrorStateEvent*)> callback);
  void subscribe(std::function<void(NoErrorStateEvent*)> callback);
  // void subscribe(std::function<void(SensorDataEvent*)> callback);
};

PubSub pubSub;
