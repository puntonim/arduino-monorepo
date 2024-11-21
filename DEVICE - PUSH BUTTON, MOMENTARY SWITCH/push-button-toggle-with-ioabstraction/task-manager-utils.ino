int _prevSecs = 0;

void printDebugTasks() {
  /* Print debug info about tasks scheduled with TaskManagerIO.
   * Usage:
   *    void loop() {
   *      printDebugTasks();
   *      taskManager.runLoop();
   *    }
   */

  const int PERIOD_SECS = 1;
  int nowSecs = floor(millis() / 1000);
  if (nowSecs >= _prevSecs + PERIOD_SECS) {

    char debugData[10];
    taskManager.checkAvailableSlots(debugData, sizeof debugData);
    int tasksCount = 0;
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

    printExtraInfo();

    Serial.println();

    _prevSecs = nowSecs;
  }
}

void printExtraInfo() {
  // noop.
}