/**
 * @example simple
 *
 * @details
 * Bare minimum implementation example. Probably you want to learn with this
 * first.
 *
 * ---
 *
 * ## main.c
 *
 * @snippet examples/simple/main.c Simple Example
 */

//! [Simple Example]

#include <errno.h> // For ENOMEM. See https://www.man7.org/linux/man-pages/man3/errno.3.html
#include <scheduler.h> // This project
#include <stdio.h>	   // For printf, puts, fputs, and similar
#include <stdlib.h>	   // For EXIT_SUCCESS, EXIT_FAILURE, and NULL
#include <time.h>	   // For timespec stuffs

// Let's say, we want to run this function in interval 1s, 3s, and 7s
void
my_handler (uint32_t delta_time)
{
	// ...which rings the bell and prints "<delta_time>ms Beat."
	// for predefined interval
	printf ("%ums Beat.\a\r\n", delta_time);
}

// So we define a list of tasks like this
const struct scheduler_task my_tasks[] = {
	{ .handler = &my_handler, .millisecond = 1000 },
	{ .handler = &my_handler, .millisecond = 3000 },
	{ .handler = &my_handler, .millisecond = 7000 },
};

// ...and let the compiler count how many tasks in that list
const unsigned int my_task_count
	= sizeof (my_tasks) / sizeof (struct scheduler_task);

// To get current time in millisecond, we need a helper function to do that
uint64_t
get_millis (void)
{
	struct timespec tms;
	timespec_get (&tms, TIME_UTC);
	return (tms.tv_sec * 1000ULL) + (tms.tv_nsec / 1000000ULL);
}

// This is your program's entry point. We added this at the end of the source
// code because we won't write header files in this example for simplicity
// reason.
int
main ()
{
	puts ("Press CTRL+C to stop.");

	scheduler_context_t my_scheduler
		= scheduler_start (my_tasks, my_task_count);

	// Check if it initialized successfully, because my_scheduler could be NULL
	// if something went wrong. Mostly because running out of memory.
	if (!my_scheduler) {
		fputs ("Failed to allocate the memory for scheduler\r\n", stderr);
		return ENOMEM;
	}

	// Loop until you hit CTRL+C
	while (1) {
		uint64_t current_millis = get_millis ();
		scheduler_iterate (my_scheduler, current_millis);

		// ATTENTION: You might notice the program takes almost 100% of CPU
		//            resource. This happens because we didn't utilized the
		//            sleep function in this loop. To understand how to utilize
		//            sleep function, see groovy_bells example.
	}

	// Ideally, you need to write this before the program exiting.
	scheduler_stop (&my_scheduler);

	return EXIT_SUCCESS;
}

//! [Simple Example]