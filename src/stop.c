#include <scheduler.h>

#include "context.h"

#include <stdlib.h>

void
scheduler_stop (scheduler_context_t *pointer_to_context)
{
	if (!pointer_to_context)
		return;

	scheduler_context_t context = *pointer_to_context;
	if (!context)
		return;

	free (context->task_timers);
	free (context);
	*pointer_to_context = NULL;
}
