/**
  * If there is AT LEAST ONE error: blink the error LED and print it on display.
  * If there are ZERO erros: switch off the error LED and clear the msg shown in the display.
  */
void ErrorManager::_handleErrors() {
  // If there is AT LEAST ONE error: blink the error LED. Else stop blinking.
  areThereErrors() ? errorLedDevice.startBlinking() : errorLedDevice.stopBlinking();
}

void ErrorManager::addDs18b20SensorError() {
  // Add the error msg to the errors list (only if we weren't already in the same error state).
  if (!isDs18b20SensorError()) {
    _errorMessageListForDisplay.push_back(_ds18b20SensorErrorMessage);
  }
  // Set the error status to true only after the prev check.
  _isDs18b20SensorError = true;
  _handleErrors();
}

void ErrorManager::removeDs18b20SensorError() {
  _isDs18b20SensorError = false;
  // Remove the error msg from the errors list.
  _errorMessageListForDisplay.remove(_ds18b20SensorErrorMessage);
  _handleErrors();
}

bool ErrorManager::isDs18b20SensorError() {
  return _isDs18b20SensorError;
}

void ErrorManager::addSht85SensorError() {
  // Add the error msg to the errors list (only if we weren't already in the same error state).
  if (!isSht85SensorError()) {
    _errorMessageListForDisplay.push_back(_sht85SensorErrorMessage);
  }
  // Set the error status to true only after the prev check.
  _isSht85SensorError = true;
  _handleErrors();
}

void ErrorManager::removeSht85SensorError() {
  _isSht85SensorError = false;
  // Remove the error msg from the errors list.
  _errorMessageListForDisplay.remove(_sht85SensorErrorMessage);
  _handleErrors();
}

bool ErrorManager::isSht85SensorError() {
  return _isSht85SensorError;
}

bool ErrorManager::areThereErrors() {
  if (isDs18b20SensorError() || isSht85SensorError()) return true;  // TODO check for the existence of AT LEAST 1 error.
  else return false;
}

/**
  * Get all the error messages as a list to be printed on the 2nd row of the
  *  16x2 display.
  */
std::list<char*> ErrorManager::getErrorMessageListForDisplay() {
  return _errorMessageListForDisplay;
}
