#ifdef TEST_INTEGRATION
	#include <stdlib.h>
	#include <stdio.h>

	#include "handlers.h"
	#include "variables.h"
	#include "get_millis.h"

	#if __has_include(<windows.h>)
		#include <Windows.h>
		#define SLEEP(t) Sleep (t)
	#else
		#include <unistd.h>
		#define SLEEP(t) usleep (t * 1000)
	#endif

int
main ()
{
	scheduler = scheduler_start (tasks, task_count);
	sub_scheduler = scheduler_start (subtasks, subtask_count);

	const uint64_t start_millis = get_millis ();
	uint64_t current_millis;
	uint32_t next_delta;

	while (1) {
		current_millis = get_millis ();
		next_delta = scheduler_iterate (scheduler, current_millis);

		if ((current_millis - start_millis) > 10000ULL)
			break;

		// Sleep function in any PC has questionable accuracy
		SLEEP (next_delta / 2);
	}

	scheduler_stop (&sub_scheduler);
	scheduler_stop (&scheduler);

	puts ("Integration test passed!");
	return EXIT_SUCCESS;
}

#endif // TEST_INTEGRATION
