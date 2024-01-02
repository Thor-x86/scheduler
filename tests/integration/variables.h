#ifndef __VARIABLES_DECLARED
#define __VARIABLES_DECLARED

#include <scheduler.h>

extern const struct scheduler_task tasks[];
extern const struct scheduler_task subtasks[];
extern const unsigned int task_count;
extern const unsigned int subtask_count;
extern scheduler_context_t scheduler;
extern scheduler_context_t sub_scheduler;

#endif // __VARIABLES_DECLARED
