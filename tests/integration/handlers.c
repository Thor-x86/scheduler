#include <scheduler.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "get_millis.h"
#include "variables.h"

static const char error_template_shifted[]
	= "FAIL: Handler with %ums interval shifted to %ums\r\n";

static const char error_template_misreport[]
	= "FAIL: Handler with %ums interval reported itself has %ums delta "
	  "time\r\n";

static uint64_t last_millis_array[8 + 3] = { 0 };

static void
base_task_handler (uint32_t interval, unsigned int index, uint32_t delta_millis)
{
	uint64_t current_millis = get_millis ();
	if (last_millis_array[index]) {
		uint32_t real_delta_millis = current_millis - last_millis_array[index];
		if (real_delta_millis != interval) {
			printf (error_template_shifted, interval, real_delta_millis);
			exit (EXIT_FAILURE);
		}
	}
	last_millis_array[index] = current_millis;

	if (delta_millis != interval) {
		printf (error_template_misreport, interval, delta_millis);
		exit (EXIT_FAILURE);
	}
}

#define GENERATE_TASK_HANDLER(interval, index)                                 \
	void task_handler_##interval##ms (uint32_t delta_millis)                   \
	{                                                                          \
		base_task_handler (interval, index, delta_millis);                     \
	}

GENERATE_TASK_HANDLER (100, 0)
GENERATE_TASK_HANDLER (130, 1)
GENERATE_TASK_HANDLER (170, 2)
GENERATE_TASK_HANDLER (230, 3)
GENERATE_TASK_HANDLER (270, 4)
GENERATE_TASK_HANDLER (333, 5)
GENERATE_TASK_HANDLER (555, 6)
GENERATE_TASK_HANDLER (777, 7)

void
task_handler_subtask ()
{
	scheduler_iterate (sub_scheduler, get_millis ());
}

#define GENERATE_SUBTASK_HANDLER(interval, index)                              \
	void subtask_handler_##interval##ms (uint32_t delta_millis)                \
	{                                                                          \
		base_task_handler (interval, index + 8, delta_millis);                 \
	}

GENERATE_SUBTASK_HANDLER (1000, 0)
GENERATE_SUBTASK_HANDLER (2500, 1)
GENERATE_SUBTASK_HANDLER (3500, 2)
