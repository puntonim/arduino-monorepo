#include "domain/main-domain.h"

#include "devices/led-device.h"
#include "settings.h"
#include "utils/error-utils.h"
#include "utils/pubsub-utils.h"

namespace bigpjtemplate {

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

void MainDomain::run() {
#if IS_DEBUG == true
  Serial.println("MainDomain - run");
#endif

  domainLedDevice.switchOn(true);

  auto nowTs = millis();
  // After 20 secs from boot, add a test error.
  if (nowTs > 19000 && nowTs <= 26000 &&
      !error_utils::errorMgr.areThereErrors()) {
    error_utils::errorMgr.addError("TEST_ERROR", "Just a test error",
                                   "Test err");
  }
  // After 25 secs from boot, remove the error.
  else if (nowTs > 26000 && nowTs <= 31000 &&
           error_utils::errorMgr.areThereErrors()) {
    error_utils::errorMgr.removeError("TEST_ERROR");
    // errorDomain.removeAllErrors();
  }
  // After 30 secs from boot, switch on the led matrix.
  else if (nowTs > 31000 && nowTs <= 36000) {
    ledMatrixDevice.switchOn();
  }
  // After 35 secs from boot, switch off the led matrix.
  else if (nowTs > 36000 && nowTs <= 41000) {
    ledMatrixDevice.switchOff();
  }

  domainLedDevice.switchOff(true);
}

}  // namespace bigpjtemplate
