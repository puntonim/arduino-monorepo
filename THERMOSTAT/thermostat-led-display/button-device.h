class ButtonDevice {

private:
  std::function<void()> _onPressCallback;

public:
  void setup();
  void setOnPressCallback(std::function<void()> callback);
  std::function<void()> getOnPressCallback();
};

ButtonDevice buttonDevice;
