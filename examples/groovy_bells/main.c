/**
 * @example groovy_bells
 *
 * @details
 * In this example program, it will demonstrate how scheduling could be useful
 * for managing a beat pattern. To try this, you have to run the program with a
 * terminal console which capable of ringing the bell (or beeping).
 *
 * ---
 *
 * ## main.c
 *
 * @snippet examples/groovy_bells/main.c Groovy Bells Main Source Code
 *
 * ---
 *
 * ## tasks.h
 *
 * @snippet examples/groovy_bells/tasks.h Groovy Bells Tasks Header
 *
 * ---
 *
 * ## tasks.c
 *
 * @snippet examples/groovy_bells/tasks.c Groovy Bells Tasks Source Code
 *
 * ---
 *
 * ## get_millis.h
 *
 * @snippet examples/groovy_bells/get_millis.h Groovy Bells get_millis Header
 *
 * ---
 *
 * ## get_millis.c
 *
 * @snippet examples/groovy_bells/get_millis.c Groovy Bells get_millis Source
 */

//! [Groovy Bells Main Source Code]

#include <errno.h>
#include <scheduler.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "get_millis.h"
#include "tasks.h"

// Sleep function in Windows is not POSIX compliant, so we need to wrap it
#if __has_include(<windows.h>)
	#include <Windows.h>
	#define SLEEP(t) Sleep (t)
#else
	#include <unistd.h>
	#define SLEEP(t) usleep (t * 1000)
#endif

const struct scheduler_task tasks[] = {
	{ .handler = &clear_terminal_task, .millisecond = 100 },
	{ .handler = &bell_1000ms_task, .millisecond = 1000 },
	{ .handler = &bell_1500ms_task, .millisecond = 1500 },
	{ .handler = &bell_2250ms_task, .millisecond = 2250 },
};

const unsigned int task_count = sizeof (tasks) / sizeof (struct scheduler_task);

int
main ()
{
	scheduler_context_t scheduler;
	uint64_t millis;
	uint32_t sleep_duration;

	puts ("Press CTRL+C to stop.");
	puts ("");

	scheduler = scheduler_start (tasks, task_count);

	puts ("|   1s  |  1.5s | 2.25s |");

	while (1) {
		millis = get_millis ();
		sleep_duration = scheduler_iterate (scheduler, millis);
		fflush (stdout);
		sleep_duration /= 2;
		SLEEP (sleep_duration);
	}

	// The program supposed to not reach here
	scheduler_stop (&scheduler);
	return EXIT_FAILURE;
}

//! [Groovy Bells Main Source Code]
