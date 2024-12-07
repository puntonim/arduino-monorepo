#include "devices/button-device.h"

#include <IoAbstraction.h>

#include "settings.h"
#include "utils/pubsub-utils.h"

namespace tstat {

// "Soft" singleton global object defined here,
//  but defined as extern and initialized in button-device.h.
DisplayButtonDevice displayButtonDevice;

/*
 * When the button is pressed, execute the callback.
 *
 * Note: this must be a static function (so it cannot be a regular class method)
 * in order to be used in `switches.addSwitch()`.
 */
static void _onButtonPress(uint8_t pin, bool isHeldDown) {
  if (isHeldDown) return;

  pubsub_utils::pubSub.publish(new pubsub_utils::ButtonPressEvent());
}

void DisplayButtonDevice::setup() {
  // Options: SWITCHES_NO_POLLING, SWITCHES_POLL_EVERYTHING,
  // SWITCHES_POLL_KEYS_ONLY.
  switches.init(ioUsingArduino(), SWITCHES_NO_POLLING, true);
  // Note: after pressing the button 1 non-repeating task is scheduled with
  // TaskManagerIO
  //  and lives forever.
  // Note: the passed function must be static, so it cannot be a regular
  // ButtonDevice method. We could
  //  solve this with a Lambda but I believe that using a regular static
  //  function is easier: switches.addSwitch(settings.PUSH_BUTTON_PIN,
  //  [](uint8_t pin, bool isHeldDown) { buttonDevice.onPress(pin, isHeldDown);
  //  }, NO_REPEAT);
  switches.addSwitch(settings::PUSH_BUTTON_PIN, _onButtonPress, NO_REPEAT);
}

}  // namespace tstat
