#include <scheduler.h>

#include "context.h"

#include <stdlib.h>
#include <string.h>

scheduler_context_t
scheduler_start (const struct scheduler_task *task_array,
				 const unsigned int task_count)
{
	if (task_count && !task_array)
		return NULL;

	scheduler_context_t context
		= (scheduler_context_t)malloc (sizeof (struct scheduler_context));
	if (!context)
		return NULL;

	memset (context, 0, sizeof (struct scheduler_context));

	context->task_timers = (uint32_t *)calloc (task_count, sizeof (uint32_t));
	if (!context->task_timers) {
		free (context);
		return NULL;
	}

	context->tasks = task_array;
	context->task_count = task_count;
	return context;
}
