#ifndef _TASK_MANAGER_UTILS_H
#define _TASK_MANAGER_UTILS_H

namespace bigpjtemplate {
namespace task_manager_utils {

void printDebugTasks(bool isVerbose = true);
void _printExtraInfo();
void cancelTask(taskid_t &taskId);

}  // namespace task_manager_utils
}  // namespace bigpjtemplate
#endif
