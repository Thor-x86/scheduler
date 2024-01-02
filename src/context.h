#ifndef __SCHEDULER_CONTEXT_DECLARED
#define __SCHEDULER_CONTEXT_DECLARED

#include <scheduler.h>

/**
 * @file context.h
 * @private
 * This is an internal header and should not be included in application source
 * code.
 */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Internally defined context
 */
struct scheduler_context {
	/// @brief Required for calculating delta time
	uint64_t last_timestamp;

	/// @brief Points to any memory region
	const struct scheduler_task *tasks;

	/// @brief Points to heap memory region
	uint32_t *task_timers;

	/// @brief Represents element count of tasks and task_countdowns
	unsigned int task_count;
};

#ifdef __cplusplus
}
#endif

#endif // __SCHEDULER_CONTEXT_DECLARED
