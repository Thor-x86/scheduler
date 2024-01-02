#ifdef TEST_UNIT
	#include <gtest/gtest.h>
	#include <scheduler.h>
	#include <src/context.h>

namespace START_funcs
{
	void
	dummy_handler (uint32_t delta_time)
	{
		(void)delta_time;
	}
}

TEST (START, NORMAL)
{
	const struct scheduler_task tasks[]
		= { { .handler = &START_funcs::dummy_handler, .millisecond = 1000 },
			{ .handler = &START_funcs::dummy_handler, .millisecond = 2000 },
			{ .handler = &START_funcs::dummy_handler, .millisecond = 3000 } };

	const unsigned int task_count
		= sizeof (tasks) / sizeof (struct scheduler_task);

	scheduler_context_t context = scheduler_start (tasks, task_count);

	EXPECT_TRUE (context);
	EXPECT_TRUE (context->task_timers);
	EXPECT_EQ (context->tasks, tasks);
	EXPECT_EQ (context->task_count, task_count);
	EXPECT_EQ (context->last_timestamp, 0);

	scheduler_stop (&context);
}

TEST (START, ABNORMAL)
{
	// Will crash if fail
	scheduler_context_t context = scheduler_start (NULL, 3);
	EXPECT_FALSE (context);
	scheduler_stop (&context);
}

TEST (START, EMPTY)
{
	scheduler_context_t context = scheduler_start (nullptr, 0);

	EXPECT_TRUE (context);
	EXPECT_TRUE (context->task_timers);
	EXPECT_EQ (context->tasks, nullptr);
	EXPECT_EQ (context->task_count, 0);
	EXPECT_EQ (context->last_timestamp, 0);

	scheduler_stop (&context);
}

#endif // TEST_UNIT
