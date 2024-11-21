class ButtonDevice {

private:

public:
  void setup();
  static void onPress(uint8_t pin, bool isHeldDown);
};

ButtonDevice buttonDevice;
