/**
 * TASK MANAGER: TaskManagerIO LIB
 * ===============================
 * It's a great library to schedule and manage tasks.
 *
 * Mind that the default behavior for a periodic task in TaskManagerIO is that
 * it is run only if the previous execution has ended already. So there will
 * never be more concurrent runs of the same periodic task.
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
void every10Sec();
void printDebugTasks(bool isVerbose = true);
void cancelTask(taskid_t& taskId);

taskid_t every10SecTaskId = TASKMGR_INVALIDID;
taskid_t once30SecTaskId = TASKMGR_INVALIDID;

void setup() {
  Serial.begin(57600);
  unsigned short i = 30;
  while (!Serial && i--) delay(1);
  Serial.println("START " __FILE__ " compiled on " __DATE__);

  // Run `every10Sec` now and then schedule it in 10 sec and every 10 sec.
  every10Sec();
  every10SecTaskId = taskManager.schedule(repeatSeconds(10), every10Sec);

  once30SecTaskId = taskManager.schedule(onceSeconds(30), [] {
    Serial.println(">>>>>>>>>>>>>>>>>>>>>>>> 30 secs!!!");
    Serial.println("Killing every10SecTaskId...");
    cancelTask(every10SecTaskId);
  });
}

void loop() {
  // To debug TaskManagerIO's tasks.
  printDebugTasks(true);

  taskManager.runLoop();
}

void every10Sec() {
  Serial.println(">>>>>>>>>>>>>>>>>>>>>>>> Every 10 sec");
  // Sleep 3 secs: this is how to sleep using TaskManagerIo.
  taskManager.yieldForMicros(3 * 1000 * 1000);
}

/**
 * Cancel a TaskManagerIO's task by its task-id.
 *
 * It does not fail if the given task-id does not exist.
 * It gets the task-id by address so it can set it to TASKMGR_INVALIDID
 *  (which is the int 65535).
 */
void cancelTask(taskid_t& taskId) {
  taskManager.cancelTask(taskId);
  taskId = TASKMGR_INVALIDID;
}

void _printExtraInfo() {
  Serial.println((String) "\nevery10SecTaskId: " +
                 (every10SecTaskId == TASKMGR_INVALIDID
                      ? "TASKMGR_INVALIDID"
                      : (String)every10SecTaskId));
  Serial.println((String) "once30SecTaskId: " +
                 (once30SecTaskId == TASKMGR_INVALIDID
                      ? "TASKMGR_INVALIDID"
                      : (String)once30SecTaskId));
}

/**
 * Print debug info about tasks scheduled with TaskManagerIO.
 * Docs:
 * https://tcmenu.github.io/documentation/arduino-libraries/taskmanager-io/task-manager-scheduling-guide/
 *
 * Usage:
 *    void loop() {
 *      printDebugTasks();
 *      taskManager.runLoop();
 *    }
 */
int _prevSecs = 0;
void printDebugTasks(bool isVerbose /* = true */) {
  const short PERIOD_SECS = 1;
  int nowSecs = floor(millis() / 1000);
  if (nowSecs >= _prevSecs + PERIOD_SECS) {
    char debugData[10];
    // Docs:
    // https://tcmenu.github.io/documentation/arduino-libraries/taskmanager-io/task-manager-scheduling-guide/
    taskManager.checkAvailableSlots(debugData, sizeof debugData);
    short tasksCount = 0;
    for (int i = 0; i < 10; i++)
      if (debugData[i] != 'F') tasksCount++;
    Serial.print((String) "#" + tasksCount + " tasks [");
    for (int i = 0; i < 10; i++) Serial.print(debugData[i]);
    Serial.println("]");

    if (isVerbose) {
      for (int i = 0; i < 10; i++) {
        if (debugData[i] != 'F') {
          auto task = taskManager.getTask(i);
          Serial.println((String) "\n\tTask id: " + i);
          Serial.println((String) "\tisRunning?: " +
                         (task->isRunning() ? "Y" : "N"));
          Serial.println((String) "\tRepeat?: " +
                         (task->isRepeating() ? "REPEAT" : "NO-REPEAT"));
          Serial.println((String) "\tEnabled?: " +
                         (task->isEnabled() ? "Y" : "N"));
          Serial.println((String) "\tisMillis?: " +
                         (task->isMillisSchedule() ? "Y" : "N"));
          Serial.println((String) "\tisMicros?: " +
                         (task->isMicrosSchedule() ? "Y" : "N"));
          Serial.println((String) "\tmicrosFromNow?: " + task->microsFromNow());
        }
      }
    }

    _printExtraInfo();

    Serial.println();

    _prevSecs = nowSecs;
  }
}
