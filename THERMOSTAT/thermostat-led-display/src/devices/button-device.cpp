#include "devices/button-device.h"

#include <IoAbstraction.h>

#include "settings.h"
#include "utils/pubsub-utils.h"

namespace tstat {

// "Soft" singleton global object defined here,
//  but defined as extern and initialized in button-device.h.
ButtonDevices buttonDevices;

/*
 * When any button is pressed, execute this callback.
 *
 * Note: this must be a static function (so it cannot be a regular class method)
 * in order to be used in `switches.addSwitch()`.
 */
static void _onAnyButtonPress(uint8_t pin, bool isHeldDown) {
  if (isHeldDown) return;

  if (pin == settings::DISPLAY_BUTTON_PIN)
    pubsub_utils::pubSub.publish(new pubsub_utils::DisplayButtonPressEvent());
  else if (pin == settings::TARGET_T_BUTTON_PIN)
    pubsub_utils::pubSub.publish(new pubsub_utils::TargetTButtonPressEvent());
  else if (pin == settings::TIMER_BUTTON_PIN)
    pubsub_utils::pubSub.publish(new pubsub_utils::TimerButtonPressEvent());
}

void ButtonDevices::setup() {
  // Options: SWITCHES_NO_POLLING, SWITCHES_POLL_EVERYTHING,
  // SWITCHES_POLL_KEYS_ONLY (rotary encoders by interrupts, buttons by poll).
  // Last arg (true) is for integrated pull-up resistors.
  // IMP: SINCE WE ARE USING 3 BUTTONS THEN WE HAVE TO USE POLLING (instead of
  //  interrupts because the limit in Arduino UNO R4 is 2 interrupts).
  switches.init(ioUsingArduino(), SWITCHES_POLL_KEYS_ONLY, true);
  // Note: the callback function must be static, so it cannot be a regular
  //  class method. We could solve this with a Lambda but I believe that
  //  using a regular static function is easier to read:
  //  switches.addSwitch(settings.DISPLAY_BUTTON_PIN, [](uint8_t pin, bool
  //  isHeldDown) { buttonDevice.onPress(pin, isHeldDown); }, NO_REPEAT);
  switches.addSwitch(settings::DISPLAY_BUTTON_PIN, _onAnyButtonPress,
                     NO_REPEAT);
  switches.addSwitch(settings::TARGET_T_BUTTON_PIN, _onAnyButtonPress,
                     NO_REPEAT);
  switches.addSwitch(settings::TIMER_BUTTON_PIN, _onAnyButtonPress, NO_REPEAT);
}

}  // namespace tstat
