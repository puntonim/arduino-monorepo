class ErrorManager {

private:
  bool _isDs18b20SensorError = false;
  char* _ds18b20SensorErrorMessage = "ERRORE T ds18b20"; // One more slot as terminated with \0.
  bool _isSht85SensorError = false;
  char* _sht85SensorErrorMessage = "ERRORE T sht85"; // One more slot as terminated with \0.
  std::list<char*> _errorMessageListForDisplay;
  void _handleErrors();

public:
  ErrorManager();
  void addDs18b20SensorError();
  void removeDs18b20SensorError();
  bool isDs18b20SensorError();
  void addSht85SensorError();
  void removeSht85SensorError();
  bool isSht85SensorError();
  bool areThereErrors();
  std::list<char *> getErrorMessageListForDisplay();
  int getErrorMessageListForDisplaySize();
  char* getErrorMessageForDisplayByIndex(int i, enum CommonException &exc);
};

ErrorManager errorManager;
