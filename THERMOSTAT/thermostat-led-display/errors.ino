void ErrorManager::_handleErrors() {
  // noop.
}

void ErrorManager::addDs18b20SensorError() {
  // Add the error msg to the errors list (only if we weren't already in the same error state).
  if (_isDs18b20SensorError) return;
  _errorMessageListForDisplay.push_back(_ds18b20SensorErrorMessage);
  // Set the error status to true only after the prev check.
  _isDs18b20SensorError = true;
  pubSub.publish(new ErrorStateEvent(_errorMessageListForDisplay));
  _handleErrors();
}

void ErrorManager::removeDs18b20SensorError() {
  // Remove the error msg from the errors list (only if we were in the same error state).
  if (!_isDs18b20SensorError) return;
  _errorMessageListForDisplay.remove(_ds18b20SensorErrorMessage);
  // Set the error status to false only after the prev check.
  _isDs18b20SensorError = false;
  pubSub.publish(new NoErrorStateEvent());
  _handleErrors();
}

bool ErrorManager::isDs18b20SensorError() {
  return _isDs18b20SensorError;
}

void ErrorManager::addSht85SensorError() {
  // Add the error msg to the errors list (only if we weren't already in the same error state).
  if (_isSht85SensorError) return;
  _errorMessageListForDisplay.push_back(_sht85SensorErrorMessage);
  // Set the error status to true only after the prev check.
  _isSht85SensorError = true;
  _handleErrors();
}

void ErrorManager::removeSht85SensorError() {
    // Remove the error msg from the errors list (only if we were in the same error state).
  if (!_isSht85SensorError) return;
  _errorMessageListForDisplay.remove(_sht85SensorErrorMessage);
  // Set the error status to false only after the prev check.
  _isSht85SensorError = false;
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
// TODO remove this when we switched everything to PUB/SUB.
std::list<char*> ErrorManager::getErrorMessageListForDisplay() {
  return _errorMessageListForDisplay;
}
