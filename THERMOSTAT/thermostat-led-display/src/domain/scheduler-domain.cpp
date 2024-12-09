/* SchedulerDomain is just a dumb cointainer class that includes a timer
 *  and the target temperature.
 * It doesn't tick the timer and doesn't check the tearget temperature against
 *  the sensor temperature.
 */
#include "domain/scheduler-domain.h"

namespace tstat {

// "Soft" singleton global object defined here,
//  but defined as extern and initialized in scheduler-domain.h.
SchedulerDomain schedulerDomain;

void SchedulerDomain::setup() {
  //
}

}  // namespace tstat