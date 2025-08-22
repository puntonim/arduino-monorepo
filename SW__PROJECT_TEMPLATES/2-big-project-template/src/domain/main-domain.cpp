#include "domain/main-domain.h"

#include "devices/led-device.h"
#include "settings.h"
#include "utils/error-utils.h"
#include "utils/pubsub-utils.h"

namespace myproject {

// "Soft" *singleton* global object defined here,
//  but defined as extern and initialized in main-domain.h.
MainDomain mainDomain;

void MainDomain::setup() {
#if IS_DEBUG == true
  Serial.println((String) "MainDomain - scheduling a run every " +
                 settings::MAIN_DOMAIN_RUN_PERIOD + " sec");
#endif

  runTaskId =
      taskManager.schedule(repeatSeconds(settings::MAIN_DOMAIN_RUN_PERIOD),
                           [] { mainDomain.run(); });
}

bool wasErrorAdded = false;
bool wasLedMatrixSwitchedOn = false;

void MainDomain::run() {
#if IS_DEBUG == true
  Serial.println("MainDomain - run");
#endif

  domainLedDevice.switchOn(true);

  auto nowTs = millis();
  // After 20 secs from boot, add a test error for 10 secs.
  if (nowTs > 20000 && !wasErrorAdded) {
    error_utils::errorMgr.addError("TEST_ERROR", "Just a test error",
                                   "Test err");
    wasErrorAdded = true;
    taskManager.schedule(onceSeconds(10), [] {
      error_utils::errorMgr.removeError("TEST_ERROR");
      // errorDomain.removeAllErrors();
    });

  }
  // After 30 secs from boot, switch on the led matrix for 10 secs.
  else if (nowTs > 30000 && !wasLedMatrixSwitchedOn) {
    ledMatrixDevice.switchOn();
    wasLedMatrixSwitchedOn = true;
    taskManager.schedule(onceSeconds(10), [] { ledMatrixDevice.switchOff(); });
  }

  domainLedDevice.switchOff(true);
}

}  // namespace myproject
