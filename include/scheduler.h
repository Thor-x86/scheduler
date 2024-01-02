#ifndef __SCHEDULER_DECLARED
#define __SCHEDULER_DECLARED

#include <stdint.h>

// Support for Windows DLL
#if __has_include(<windows.h>)
	#if defined(__SCHEDULER_INTERNAL)
		#if defined(TEST_UNIT) || defined(TEST_INTEGRATION)
			#define LINKED_FUNCTION extern
		#else
			#define LINKED_FUNCTION extern __declspec (dllexport)
		#endif
	#else
		#define LINKED_FUNCTION __declspec (dllimport)
	#endif
#else // __has_include(<windows.h>)
	#define LINKED_FUNCTION extern
#endif // __has_include(<windows.h>)

/**
 * @file scheduler.h
 * Contains type definitions and functions from scheduler module which can be
 * applied on main source code.
 */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Private memory type declaration
 *
 * @details
 * This variable exists only for type enforcement. You shouldn't use
 * struct scheduler_context directly on your application source code.
 */
struct scheduler_context;

/**
 * @brief Points to an allocated memory for scheduler module
 *
 * @details
 * The reason why this library didn't (and won't) store global variables
 * because it would cause trouble if you want to do multiple independent
 * scheduling processes. You **MUST** clean it with scheduler_stop
 * function if not in use anymore.
 */
typedef struct scheduler_context *scheduler_context_t;

/**
 * @brief Defines how task handler function looks like
 * @param delta_millisecond tells how long time since last call actually takes
 *
 * @details
 * The term "task handler" means a function which need to be called when the
 * interval just elapsed.
 *
 * **Example**
 *
 * @code{.c}
 * // This is a correct task handler
 * void example_task(uint32_t delta_millisecond)
 * {
 *     // ...
 * }
 *
 * // This is not a task handler
 * uint16_t wrong_task_example(uint8_t a, int64_t b)
 * {
 *     // ...
 * }
 * @endcode
 */
typedef void (*scheduler_task_handler_t) (uint32_t delta_millisecond);

/**
 * @brief Represents a single repetitive task
 *
 * @details
 * Task is a sequence of procedure that need to be repeated in a defined
 * interval. Single task has an information about when to do the procedure and
 * what procedure to be done.
 *
 * **Example**
 *
 * @code{.c}
 * void first_task(uint32_t delta_millis) { ... }
 * void second_task(uint32_t delta_millis) { ... }
 *
 * const struct scheduler_task my_tasks[] = {
 *     {
 *         .handler = &first_task,
 *         .millisecond = 1000
 *     },
 *     {
 *         .handler = &second_task,
 *         .millisecond = 1500
 *     }
 * };
 * @endcode
 */
struct scheduler_task {
	/// @brief Points which function to call when the interval elapsed
	scheduler_task_handler_t handler;

	/// @brief Task interval in milliseconds unit
	uint32_t millisecond;
};

/**
 * @brief Initiate a single instance of scheduler
 * @param task_array defined as array of struct scheduler_task
 * @param task_count represents number of tasks in task_array
 * @return scheduler_context_t if task_array is valid, otherwise NULL
 *
 * @details
 * This function takes list of tasks from task_array parameter and then
 * allocates a region of memory to take a count for each task. Keep in mind
 * that the scheduler_start function won't do the iteration. scheduler_iterate
 * does that.
 *
 * **Example**
 *
 * @code{.c}
 * void first_task(uint32_t delta_millis) { ... }
 * void second_task(uint32_t delta_millis) { ... }
 *
 * const struct scheduler_task my_tasks[] = {
 *     {
 *         .handler = &first_task,
 *         .millisecond = 1000
 *     },
 *     {
 *         .handler = &second_task,
 *         .millisecond = 1500
 *     }
 * };
 *
 * int main() {
 *     scheduler_context_t my_scheduler = scheduler_start(my_tasks, 2);
 *
 *     // ...
 * }
 * @endcode
 */
LINKED_FUNCTION scheduler_context_t scheduler_start (
	const struct scheduler_task *task_array, const unsigned int task_count);

/**
 * @brief Call this function repetitively
 * @param context came from scheduler_start
 * @param timestamp in millisecond since program start or UNIX epoch
 * @return uint32_t as remaining milliseconds toward nearest task
 *
 * @details
 * It checks if there is any ready task to be called and updates the counter
 * for each task. If the task is ready, this function also calls the handler.
 * Remember to keep every task handler's execution time short as possible, or
 * the program would experience overdue task or worst, the program hangs.
 *
 * **Example**
 *
 * @code{.c}
 * #include <time.h>
 *
 * // Sleep function in Windows is quite different
 * #if __has_include(<windows.h>)
 *     #include <Windows.h>
 *     #define SLEEP(t) Sleep(t)
 * #else
 *     #include <unistd.h>
 *     #define SLEEP(t) usleep(t*1000)
 * #endif
 *
 * // ...
 *
 * int main () {
 *     scheduler_context_t my_scheduler = scheduler_start(my_tasks, 2);
 *     struct timespec tms = {0};
 *
 *     // ...
 *
 *     while (1) {
 *         timespec_get(&tms, TIME_UTC);
 *         uint64_t timestamp = (tms.tv_sec * 1000) + (tms.tv_nsec / 1000000);
 *
 *         uint32_t remaining_milliseconds = scheduler_iterate(my_scheduler,
 *                                                             timestamp);
 *         SLEEP(remaining_milliseconds);
 *     }
 *
 *     // ...
 * }
 * @endcode
 */
LINKED_FUNCTION uint32_t scheduler_iterate (scheduler_context_t context,
											const uint64_t timestamp);

/**
 * @brief Clean-up the started scheduler then NULLify the context
 * @param pointer_to_context must has ampersand prefix, as example &my_scheduler
 *
 * @details
 * Ensures the scheduler was cleaned-up from memory for safety. So accidental
 * scheduler_iterate call won't cause segmentation fault. This happens by
 * freeing the context and NULLify the context because scheduler_iterate will
 * ignore NULL context. Therefore, the parameter is pointer_to_context, not
 * regular context parameter. See the example for better explanation.
 *
 * **Example**
 *
 * @code{.c}
 * // ...
 *
 * int main() {
 *     scheduler_context_t my_scheduler = scheduler_start(my_tasks, 2);
 *
 *     // ...
 *
 *     scheduler_stop(&my_scheduler);
 *     return 0;
 * }
 * @endcode
 */
LINKED_FUNCTION void scheduler_stop (scheduler_context_t *pointer_to_context);

#ifdef __cplusplus
}
#endif

#undef LINKED_FUNCTION

#endif // __SCHEDULER_DECLARED
