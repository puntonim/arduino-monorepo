const int PUSH_BUTTON_PIN = 2;

void setupButton() {
  // Options: SWITCHES_NO_POLLING, SWITCHES_POLL_EVERYTHING, SWITCHES_POLL_KEYS_ONLY.
  switches.init(ioUsingArduino(), SWITCHES_NO_POLLING, true);
  // Note: after pressing the button 1 non-repeating task is scheduled with TaskManagerIO
  //  and lives forever.
  switches.addSwitch(PUSH_BUTTON_PIN, onButtonTriggered, NO_REPEAT);
}

void onButtonTriggered(uint8_t pin, bool isHeldDown) {
  if (isHeldDown) return;

  switchOnDisplay();
}
