#include <scheduler.h>
#include <stdlib.h>

#include "handlers.h"

const struct scheduler_task tasks[]
	= { { .handler = &task_handler_100ms, .millisecond = 100 },
		{ .handler = &task_handler_130ms, .millisecond = 130 },
		{ .handler = &task_handler_170ms, .millisecond = 170 },
		{ .handler = &task_handler_230ms, .millisecond = 230 },
		{ .handler = &task_handler_270ms, .millisecond = 270 },
		{ .handler = &task_handler_333ms, .millisecond = 333 },
		{ .handler = &task_handler_555ms, .millisecond = 555 },
		{ .handler = &task_handler_777ms, .millisecond = 777 },
		{ .handler = &task_handler_subtask, .millisecond = 500 } };

const struct scheduler_task subtasks[]
	= { { .handler = &subtask_handler_1000ms, .millisecond = 1000 },
		{ .handler = &subtask_handler_2500ms, .millisecond = 2500 },
		{ .handler = &subtask_handler_3500ms, .millisecond = 3500 } };

const unsigned int task_count = sizeof (tasks) / sizeof (struct scheduler_task);
const unsigned int subtask_count
	= sizeof (subtasks) / sizeof (struct scheduler_task);

scheduler_context_t scheduler = NULL;
scheduler_context_t sub_scheduler = NULL;
