#include <scheduler.h>

#include "context.h"

#include <stdlib.h>
#include <string.h>

#define MAX_64_BITS 0xFFFFFFFFFFFFFFFFULL
#define HALF_MAX_64_BITS 0x7FFFFFFFFFFFFFFFULL

uint32_t
scheduler_iterate (scheduler_context_t context, const uint64_t timestamp)
{
	if (!context)
		return 0U;

	if (!context->last_timestamp)
		context->last_timestamp = timestamp;

	uint64_t delta_millis = timestamp - context->last_timestamp;
	uint32_t next_task_millis = 0;

	context->last_timestamp = timestamp;

	// Detect timestamp overflow and fix it
	if (delta_millis > HALF_MAX_64_BITS)
		delta_millis = MAX_64_BITS - delta_millis;

	for (int i = 0, n = context->task_count; i < n; i++) {
		context->task_timers[i] += delta_millis;

		if (context->task_timers[i] >= context->tasks[i].millisecond) {
			if (context->tasks[i].handler)
				context->tasks[i].handler (context->task_timers[i]);

			if (context->tasks[i].millisecond)
				do
					context->task_timers[i] -= context->tasks[i].millisecond;
				while (context->task_timers[i] > context->tasks[i].millisecond);
			else
				context->task_timers[i] = 0;
		}

		uint32_t remaining
			= context->tasks[i].millisecond - context->task_timers[i];

		if ((remaining < next_task_millis) || !next_task_millis)
			next_task_millis = remaining;
	}

	return next_task_millis;
}
