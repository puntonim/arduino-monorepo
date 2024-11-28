namespace task_manager_utils {

int _prevSecs = 0;

/**
  * Print debug info about tasks scheduled with TaskManagerIO.
  *
  * Usage:
  *    void loop() {
  *      printDebugTasks();
  *      taskManager.runLoop();
  *    }
  */
void printDebugTasks() {
#if IS_DEBUG == true
  const short PERIOD_SECS = 1;
  int nowSecs = floor(millis() / 1000);
  if (nowSecs >= _prevSecs + PERIOD_SECS) {

    char debugData[10];
    taskManager.checkAvailableSlots(debugData, sizeof debugData);
    short tasksCount = 0;
    for (int i = 0; i < 10; i++)
      if (debugData[i] != 'F') tasksCount++;
    Serial.print((String) "#" + tasksCount + " tasks [");
    for (int i = 0; i < 10; i++) Serial.print(debugData[i]);
    Serial.println("]");

    for (int i = 0; i < 10; i++) {
      if (debugData[i] != 'F') {
        auto task = taskManager.getTask(i);
        Serial.println((String) "\n\tTask id: " + i);
        Serial.println((String) "\tRepeat?: " + (task->isRepeating() ? "REPEAT" : "NO-REPEAT"));
        Serial.println((String) "\tEnabled?: " + (task->isEnabled() ? "Y" : "N"));
        Serial.println((String) "\tisMillis?: " + (task->isMillisSchedule() ? "Y" : "N"));
        Serial.println((String) "\tisMicros?: " + (task->isMicrosSchedule() ? "Y" : "N"));
        Serial.println((String) "\tmicrosFromNow?: " + task->microsFromNow());
      }
    }

    _printExtraInfo();

    Serial.println();

    _prevSecs = nowSecs;
  }
#endif
}

void _printExtraInfo() {
#if IS_DEBUG == true
  Serial.println((String) "\nswitchOffDisplayTaskId: " + switchOffDisplayTaskId);
  Serial.println((String) "displayDataTaskId: " + displayDataTaskId);
  Serial.println((String) "runTaskId: " + runTaskId);
  Serial.println((String) "blinkHeatingLedTaskId: " + blinkHeatingLedTaskId);
  Serial.println((String) "blinkErrorLedTaskId: " + blinkErrorLedTaskId);
#endif
}

/**
 * Cancel a TaskManagerIO's task by its task-id.
 *
 * It does not fail if the given task-id does not exist.
 * It gets the task-id by address so it can set it to TASKMGR_INVALIDID
 *  (which is the int 65535).
 */
void cancelTask(taskid_t &taskId) {
  taskManager.cancelTask(taskId);
  taskId = TASKMGR_INVALIDID;
}

}
