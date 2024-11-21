void ButtonDevice::setup() {
  // Options: SWITCHES_NO_POLLING, SWITCHES_POLL_EVERYTHING, SWITCHES_POLL_KEYS_ONLY.
  switches.init(ioUsingArduino(), SWITCHES_NO_POLLING, true);
  // Note: after pressing the button 1 non-repeating task is scheduled with TaskManagerIO
  //  and lives forever.
  switches.addSwitch(PUSH_BUTTON_PIN, buttonDevice.onPress, NO_REPEAT);
}

/*
 * When the button is pressed, switch on the display.
 * 
 * Note: it's a STATIC method as it has to be used in `switches.addSwitch()`.
 */
void ButtonDevice::onPress(uint8_t pin, bool isHeldDown) {
  if (isHeldDown) return;

  displayDevice.switchOn();
}
