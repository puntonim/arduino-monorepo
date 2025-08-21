#include "devices/rotary-encoder-device.h"

#include <IoAbstraction.h>

#include "devices/display-device.h"
#include "settings.h"
#include "utils/pubsub-utils.h"

namespace bigpjtemplate {

// "Soft" singleton global object defined here,
//  but defined as extern and initialized in rotary-encoder-device.h.
RotaryEncoderDevices rotaryEncoderDevices;

/*
 * When any rotary encoder is pressed, execute this callback.
 *
 * Note: callbacks must be static functions (so this cannot be a regular class
 *  method) in order to be used in `switches.addSwitch()`.
 */
static void _onAnyRotaryPress(const uint8_t pin, const bool isHeldDown) {
  //
  // IMP: MIND THAT THIS FUNCTION IS WRITTEN AS THERE WERE MULTIPLE
  //  ROTARY ENCODERS (like in the project smart-thermostat) but actually in
  //  this template there is only 1 rotary.
  //

  if (isHeldDown) {
    // Detect when we hold both buttons.
    // Note that we can NOT do this only for one rotary (and no the other)
    //  because, in that case, the other one would trigger a regular held.
    for (auto i : {settings::TIMER_ROTARY_SW_PIN}) {
      if ((i != pin) && switches.isSwitchPressed(i)) {
        // We need this "if" statement otherwise otherwise we would execute
        //  twice, one for each button held.
        if (pin == settings::TIMER_ROTARY_SW_PIN) {
          pubsub_utils::pubSub.publish(
              new pubsub_utils::AllRotariesHoldEvent());
        }
        return;
      }
    }

    pubsub_utils::pubSub.publish(new pubsub_utils::AnyRotaryHoldEvent());
  };

  if (pin == settings::TIMER_ROTARY_SW_PIN) {
    pubsub_utils::pubSub.publish(new pubsub_utils::TimerRotaryPressEvent());
  }
}

static void _onTimerRotaryChange(const int value) {
  pubsub_utils::pubSub.publish(
      new pubsub_utils::TimerRotaryRotationEvent(value, displayDevice.isOn()));
}

void RotaryEncoderDevices::setup() {
  // Setup the rotary encoder press as a regular switch button.
  // Options: SWITCHES_NO_POLLING, SWITCHES_POLL_EVERYTHING,
  // SWITCHES_POLL_KEYS_ONLY (rotary encoders by interrupts, buttons by poll).
  // Mind that there is a limit in the number of devices that can work with
  //  interrupts simultaneously. If using SWITCHES_NO_POLLING with 2 encoders
  //  then the button press does not work, while SWITCHES_POLL_KEYS_ONLY works.
  // Last arg (true) is for integrated pull-up resistors.
  switches.init(ioUsingArduino(), SWITCHES_POLL_KEYS_ONLY, true);
  switches.addSwitch(settings::TIMER_ROTARY_SW_PIN, _onAnyRotaryPress,
                     NO_REPEAT);

  // Setup the rotary encoder rotation.
  // Options for HA acceleration mode: HWACCEL_SLOWER, HWACCEL_REGULAR,
  // HWACCEL_NONE.
  // Note: switch the order of the params ROTARY_DT_PIN and ROTARY_CLK_PIN
  //  if you want to invert the clockwise-counterclockwise behavior.
  _pTimerRotary = new HardwareRotaryEncoder(settings::TIMER_ROTARY_DT_PIN,
                                            settings::TIMER_ROTARY_CLK_PIN,
                                            _onTimerRotaryChange, HWACCEL_NONE);
  // Indexed array to hold all the rotaries.
  switches.setEncoder(0, _pTimerRotary);
  // Configure the rotary.
  // Using maxValue=0 causes the callback to get 1 on every clockwise rotation,
  //  and -1 on every counterclockwise rotation.
  _pTimerRotary->changePrecision(
      0,      // max value.
      0,      // starting value.
      false,  // Wrap around after hitting min and max.
      1       // step size.
  );
}

}  // namespace bigpjtemplate
