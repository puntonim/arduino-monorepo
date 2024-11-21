const int PUSH_BUTTON_PIN = 2;


class ButtonDevice {

private:

public:
  void setup();
  static void onPress(uint8_t pin, bool isHeldDown);
};

ButtonDevice buttonDevice;
