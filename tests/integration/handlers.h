#ifndef __HANDLERS_DECLARED
#define __HANDLERS_DECLARED

#include <stdint.h>

extern void task_handler_100ms (uint32_t delta_millis);
extern void task_handler_130ms (uint32_t delta_millis);
extern void task_handler_170ms (uint32_t delta_millis);
extern void task_handler_230ms (uint32_t delta_millis);
extern void task_handler_270ms (uint32_t delta_millis);
extern void task_handler_333ms (uint32_t delta_millis);
extern void task_handler_555ms (uint32_t delta_millis);
extern void task_handler_777ms (uint32_t delta_millis);

extern void task_handler_subtask ();
extern void subtask_handler_1000ms (uint32_t delta_millis);
extern void subtask_handler_2500ms (uint32_t delta_millis);
extern void subtask_handler_3500ms (uint32_t delta_millis);

#endif // __HANDLERS_DECLARED
