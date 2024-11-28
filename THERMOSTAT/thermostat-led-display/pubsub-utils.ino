namespace pubsub_utils {

void PubSub::publish(ButtonPressEvent* pEvent) {
  for (auto& callback : _buttonPressSubCallbacks) {
    callback(pEvent);
  }
}

void PubSub::publish(HeatingStatusChangeEvent* pEvent) {
  for (auto& callback : _heatingStatusChangeSubCallbacks) {
    callback(pEvent);
  }
}

void PubSub::publish(ErrorStatusChangeEvent* pEvent) {
  for (auto& callback : _errorStatusChangeSubCallbacks) {
    callback(pEvent);
  }
}

// void PubSub::publish(SensorDataEvent* pEvent) {
//   for (auto& callback : _sensorDataSubCallbacks) {
//     callback(pEvent);
//   }
// }

void PubSub::subscribe(std::function<void(ButtonPressEvent*)> callback) {
  _buttonPressSubCallbacks.push_back(callback);
}

void PubSub::subscribe(std::function<void(HeatingStatusChangeEvent*)> callback) {
  _heatingStatusChangeSubCallbacks.push_back(callback);
}

void PubSub::subscribe(std::function<void(ErrorStatusChangeEvent*)> callback) {
  _errorStatusChangeSubCallbacks.push_back(callback);
}

// PubSub::void subscribe(std::function<void(SensorDataEvent*)> callback) {
//   _sensorDataSubCallbacks.push_back(callback);
// }

}