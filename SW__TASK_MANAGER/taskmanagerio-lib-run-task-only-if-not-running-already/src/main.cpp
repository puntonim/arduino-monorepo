/**
 * TASK MANAGER: RUN TASK ONLY IF NOT RUNNING ALREADY
 * ==================================================
 * The default behavior for a periodic task in TaskManagerIO is that it is run
 * only if the previous execution has ended already. So there will never be more
 *  concurrent runs of the same periodic task.
 * This examples proves this default behavior.
 *
 * Docs:
 *  - https://tcmenu.github.io/documentation/arduino-libraries/taskmanager-io/
 *
 * Libs:
 *  - TaskManagerIO by davetcc
 *    https://github.com/TcMenu/TaskManagerIO
 */

#include <Arduino.h>
#include <TaskManagerIO.h>

// Functions declaration w/ default args.
void every1Sec();

taskid_t every1SecTaskId = TASKMGR_INVALIDID;
unsigned short nRuns = 0;

void setup() {
  Serial.begin(57600);
  unsigned short i = 30;
  while (!Serial && i--) delay(1);
  Serial.println("START " __FILE__ " compiled on " __DATE__);

  // Run `every1Sec` now and then schedule it in 1 sec and every 1 sec.
  every1Sec();
  every1SecTaskId = taskManager.schedule(repeatSeconds(1), every1Sec);

  taskManager.schedule(repeatSeconds(1), [] { Serial.println("."); });
}

void loop() { taskManager.runLoop(); }

void every1Sec() {
  Serial.println(">>>>>>>>>>>>>>>>>>>>>>>> Every 1 sec: " + (String)nRuns);
  nRuns++;
  // Sleep 10 secs: this is how to sleep using TaskManagerIo. It's blocking.
  taskManager.yieldForMicros(10 * 1000 * 1000);
}
