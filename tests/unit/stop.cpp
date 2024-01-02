#ifdef TEST_UNIT
	#include <gtest/gtest.h>
	#include <scheduler.h>
	#include <src/context.h>

namespace STOP_funcs
{
	void
	dummy_handler (uint32_t delta_time)
	{
		(void)delta_time;
	}
}

namespace STOP_consts
{
	const struct scheduler_task tasks[]
		= { { .handler = &STOP_funcs::dummy_handler, .millisecond = 1000 },
			{ .handler = &STOP_funcs::dummy_handler, .millisecond = 2000 },
			{ .handler = &STOP_funcs::dummy_handler, .millisecond = 3000 } };

	const unsigned int task_count
		= sizeof (tasks) / sizeof (struct scheduler_task);
}

TEST (STOP, NORMAL)
{
	scheduler_context_t context
		= scheduler_start (STOP_consts::tasks, STOP_consts::task_count);

	// Will crash if fail
	scheduler_stop (&context);
	EXPECT_FALSE (context);
}

TEST (STOP, ABNORMAL) { scheduler_stop (nullptr); }

TEST (STOP, TWICE)
{
	scheduler_context_t context
		= scheduler_start (STOP_consts::tasks, STOP_consts::task_count);

	scheduler_stop (&context);
	// Will crash if fail
	scheduler_stop (&context);
	EXPECT_FALSE (context);
}

#endif // TEST_UNIT
