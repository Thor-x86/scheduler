#ifdef TEST_UNIT
	#include <gtest/gtest.h>
	#include <scheduler.h>
	#include <src/context.h>

	#define OVERFLOW_START (0xFFFFFFFFFFFFFFFFULL - 5000ULL)
	#define OVERFLOW_END 5000ULL

namespace ITERATE_vars
{
	uint32_t reported_delta_time = 0;
	uint32_t reported_delta_time_1 = 0;
	uint32_t reported_delta_time_2 = 0;
}

namespace ITERATE_funcs
{
	void
	dummy_handler (uint32_t delta_time)
	{
		ITERATE_vars::reported_delta_time = delta_time;
	}
	void
	dummy_handler_1 (uint32_t delta_time)
	{
		ITERATE_vars::reported_delta_time_1 = delta_time;
	}
	void
	dummy_handler_2 (uint32_t delta_time)
	{
		ITERATE_vars::reported_delta_time_2 = delta_time;
	}
}

namespace ITERATE_consts
{
	const char label_index[] = "Where i = ";
}

TEST (ITERATE, EMPTY)
{
	struct scheduler_context context = { .last_timestamp = 0,
										 .tasks = nullptr,
										 .task_timers = nullptr,
										 .task_count = 0 };

	for (int i = 0; i < 10000; i++) {
		uint32_t result = scheduler_iterate (&context, i);
		ASSERT_EQ (result, 0) << ITERATE_consts::label_index << i;
	}
}

TEST (ITERATE, EMPTY_OVERFLOW)
{
	struct scheduler_context context = { .last_timestamp = 0,
										 .tasks = nullptr,
										 .task_timers = nullptr,
										 .task_count = 0 };

	for (uint64_t i = OVERFLOW_START; i > OVERFLOW_END; i++) {
		uint32_t result = scheduler_iterate (&context, i);
		ASSERT_EQ (result, 0) << ITERATE_consts::label_index << i;
	}
}

TEST (ITERATE, SINGLE_INSTANT)
{
	const struct scheduler_task tasks[1]
		= { { .handler = &ITERATE_funcs::dummy_handler, .millisecond = 0 } };

	uint32_t task_timers[1] = { 0U };
	struct scheduler_context context = { .last_timestamp = 0,
										 .tasks = tasks,
										 .task_timers = task_timers,
										 .task_count = 1 };

	ITERATE_vars::reported_delta_time = 0;

	for (int i = 0; i < 10000; i++) {
		uint32_t result = scheduler_iterate (&context, i);
		ASSERT_EQ (task_timers[0], 0) << ITERATE_consts::label_index << i;
		ASSERT_EQ (result, 0) << ITERATE_consts::label_index << i;
		ASSERT_EQ (ITERATE_vars::reported_delta_time, i > 1)
			<< ITERATE_consts::label_index << i;
	}
}

TEST (ITERATE, SINGLE_INSTANT_OVERFLOW)
{
	const struct scheduler_task tasks[1]
		= { { .handler = &ITERATE_funcs::dummy_handler, .millisecond = 0 } };

	uint32_t task_timers[1] = { 0U };
	struct scheduler_context context = { .last_timestamp = 0,
										 .tasks = tasks,
										 .task_timers = task_timers,
										 .task_count = 1 };

	ITERATE_vars::reported_delta_time = 0;

	for (uint64_t i = OVERFLOW_START; i > OVERFLOW_END; i++) {
		uint32_t result = scheduler_iterate (&context, i);
		ASSERT_EQ (task_timers[0], 0) << ITERATE_consts::label_index << i;
		ASSERT_EQ (result, 0) << ITERATE_consts::label_index << i;
		ASSERT_EQ (ITERATE_vars::reported_delta_time, i > OVERFLOW_START)
			<< ITERATE_consts::label_index << i;
	}
}

TEST (ITERATE, MULTI_INSTANT)
{
	const struct scheduler_task tasks[]
		= { { .handler = &ITERATE_funcs::dummy_handler, .millisecond = 0 },
			{ .handler = &ITERATE_funcs::dummy_handler_1, .millisecond = 0 },
			{ .handler = &ITERATE_funcs::dummy_handler_2, .millisecond = 0 } };
	const unsigned int task_count
		= sizeof (tasks) / sizeof (struct scheduler_task);

	uint32_t task_timers[task_count] = { 0 };
	struct scheduler_context context = { .last_timestamp = 0,
										 .tasks = tasks,
										 .task_timers = task_timers,
										 .task_count = task_count };

	ITERATE_vars::reported_delta_time = 0;
	ITERATE_vars::reported_delta_time_1 = 0;
	ITERATE_vars::reported_delta_time_2 = 0;

	for (int i = 0; i < 10000; i++) {
		uint32_t result = scheduler_iterate (&context, i);

		ASSERT_EQ (task_timers[0], 0) << ITERATE_consts::label_index << i;
		ASSERT_EQ (task_timers[1], 0) << ITERATE_consts::label_index << i;
		ASSERT_EQ (task_timers[2], 0) << ITERATE_consts::label_index << i;

		ASSERT_EQ (result, 0) << ITERATE_consts::label_index << i;

		ASSERT_EQ (ITERATE_vars::reported_delta_time, i > 1)
			<< ITERATE_consts::label_index << i;
		ASSERT_EQ (ITERATE_vars::reported_delta_time_1, i > 1)
			<< ITERATE_consts::label_index << i;
		ASSERT_EQ (ITERATE_vars::reported_delta_time_2, i > 1)
			<< ITERATE_consts::label_index << i;
	}
}

TEST (ITERATE, MULTI_INSTANT_OVERFLOW)
{
	const struct scheduler_task tasks[]
		= { { .handler = &ITERATE_funcs::dummy_handler, .millisecond = 0 },
			{ .handler = &ITERATE_funcs::dummy_handler_1, .millisecond = 0 },
			{ .handler = &ITERATE_funcs::dummy_handler_2, .millisecond = 0 } };
	const unsigned int task_count
		= sizeof (tasks) / sizeof (struct scheduler_task);

	uint32_t task_timers[task_count] = { 0 };
	struct scheduler_context context = { .last_timestamp = 0,
										 .tasks = tasks,
										 .task_timers = task_timers,
										 .task_count = task_count };

	ITERATE_vars::reported_delta_time = 0;
	ITERATE_vars::reported_delta_time_1 = 0;
	ITERATE_vars::reported_delta_time_2 = 0;

	for (uint64_t i = OVERFLOW_START; i < OVERFLOW_END; i++) {
		uint32_t result = scheduler_iterate (&context, i);

		ASSERT_EQ (task_timers[0], 0) << ITERATE_consts::label_index << i;
		ASSERT_EQ (task_timers[1], 0) << ITERATE_consts::label_index << i;
		ASSERT_EQ (task_timers[2], 0) << ITERATE_consts::label_index << i;

		ASSERT_EQ (result, 0) << ITERATE_consts::label_index << i;

		ASSERT_EQ (ITERATE_vars::reported_delta_time, i > OVERFLOW_START)
			<< ITERATE_consts::label_index << i;
		ASSERT_EQ (ITERATE_vars::reported_delta_time_1, i > OVERFLOW_START)
			<< ITERATE_consts::label_index << i;
		ASSERT_EQ (ITERATE_vars::reported_delta_time_2, i > OVERFLOW_START)
			<< ITERATE_consts::label_index << i;
	}
}

TEST (ITERATE, SINGLE_DELAYED)
{
	const struct scheduler_task tasks[1]
		= { { .handler = &ITERATE_funcs::dummy_handler, .millisecond = 110U } };

	uint32_t task_timers[1] = { 0U };
	struct scheduler_context context = { .last_timestamp = 0,
										 .tasks = tasks,
										 .task_timers = task_timers,
										 .task_count = 1 };

	ITERATE_vars::reported_delta_time = 0;

	for (int i = 0; i < 5000; i++) {
		uint32_t result = scheduler_iterate (&context, i);

		if (i > 0) {
			const uint32_t expected_timer = (i - 1) % 110U;
			ASSERT_EQ (task_timers[0], expected_timer)
				<< ITERATE_consts::label_index << i;
			ASSERT_EQ (result, 110U - expected_timer)
				<< ITERATE_consts::label_index << i;

			if (expected_timer || i == 1)
				ASSERT_EQ (ITERATE_vars::reported_delta_time, 0)
					<< ITERATE_consts::label_index << i;
			else
				ASSERT_EQ (ITERATE_vars::reported_delta_time, 110U)
					<< ITERATE_consts::label_index << i;

			ITERATE_vars::reported_delta_time = 0;
		} else {
			ASSERT_EQ (task_timers[0], 0) << ITERATE_consts::label_index << i;
			ASSERT_EQ (result, 110U) << ITERATE_consts::label_index << i;
			ASSERT_EQ (ITERATE_vars::reported_delta_time, 0)
				<< ITERATE_consts::label_index << i;
		}
	}
}

TEST (ITERATE, SINGLE_DELAYED_OVERFLOW)
{
	const struct scheduler_task tasks[1]
		= { { .handler = &ITERATE_funcs::dummy_handler, .millisecond = 110U } };

	uint32_t task_timers[1] = { 0U };
	struct scheduler_context context = { .last_timestamp = 0,
										 .tasks = tasks,
										 .task_timers = task_timers,
										 .task_count = 1 };

	ITERATE_vars::reported_delta_time = 0;

	for (uint64_t i = OVERFLOW_START; i < OVERFLOW_END; i++) {
		uint32_t result = scheduler_iterate (&context, i);

		const uint32_t expected_timer = (OVERFLOW_START - i - 1) % 110U;
		ASSERT_EQ (task_timers[0], expected_timer)
			<< ITERATE_consts::label_index << i;
		ASSERT_EQ (result, 110U - expected_timer)
			<< ITERATE_consts::label_index << i;

		if (expected_timer)
			ASSERT_EQ (ITERATE_vars::reported_delta_time, 0)
				<< ITERATE_consts::label_index << i;
		else
			ASSERT_EQ (ITERATE_vars::reported_delta_time, 110U)
				<< ITERATE_consts::label_index << i;

		ITERATE_vars::reported_delta_time = 0;
	}
}

TEST (ITERATE, MULTI_DELAYED)
{
	const struct scheduler_task tasks[]
		= { { .handler = &ITERATE_funcs::dummy_handler, .millisecond = 110 },
			{ .handler = &ITERATE_funcs::dummy_handler_1, .millisecond = 177 },
			{ .handler = &ITERATE_funcs::dummy_handler_2,
			  .millisecond = 333 } };

	const unsigned int task_count
		= sizeof (tasks) / sizeof (struct scheduler_task);

	uint32_t task_timers[task_count] = { 0 };
	struct scheduler_context context = { .last_timestamp = 0,
										 .tasks = tasks,
										 .task_timers = task_timers,
										 .task_count = task_count };

	ITERATE_vars::reported_delta_time = 0;
	ITERATE_vars::reported_delta_time_1 = 0;
	ITERATE_vars::reported_delta_time_2 = 0;

	for (int i = 0; i < 10000; i++) {
		uint32_t result = scheduler_iterate (&context, i);

		if (i > 0) {
			const uint32_t expected_timer = (i - 1) % 110U;
			const uint32_t expected_timer_1 = (i - 1) % 177U;
			const uint32_t expected_timer_2 = (i - 1) % 333U;

			uint32_t smallest_remaining = 110U - expected_timer;
			if (177U - expected_timer_1 < smallest_remaining)
				smallest_remaining = 177U - expected_timer_1;
			if (333U - expected_timer_2 < smallest_remaining)
				smallest_remaining = 333U - expected_timer_2;

			ASSERT_EQ (task_timers[0], expected_timer)
				<< ITERATE_consts::label_index << i;
			ASSERT_EQ (task_timers[1], expected_timer_1)
				<< ITERATE_consts::label_index << i;
			ASSERT_EQ (task_timers[2], expected_timer_2)
				<< ITERATE_consts::label_index << i;

			ASSERT_EQ (result, smallest_remaining)
				<< ITERATE_consts::label_index << i;

			if (expected_timer || i == 1)
				ASSERT_EQ (ITERATE_vars::reported_delta_time, 0)
					<< ITERATE_consts::label_index << i;
			else
				ASSERT_EQ (ITERATE_vars::reported_delta_time, 110U)
					<< ITERATE_consts::label_index << i;

			if (expected_timer_1 || i == 1)
				ASSERT_EQ (ITERATE_vars::reported_delta_time_1, 0)
					<< ITERATE_consts::label_index << i;
			else
				ASSERT_EQ (ITERATE_vars::reported_delta_time_1, 177U)
					<< ITERATE_consts::label_index << i;

			if (expected_timer_2 || i == 1)
				ASSERT_EQ (ITERATE_vars::reported_delta_time_2, 0)
					<< ITERATE_consts::label_index << i;
			else
				ASSERT_EQ (ITERATE_vars::reported_delta_time_2, 333U)
					<< ITERATE_consts::label_index << i;

			ITERATE_vars::reported_delta_time = 0;
			ITERATE_vars::reported_delta_time_1 = 0;
			ITERATE_vars::reported_delta_time_2 = 0;
		} else {
			ASSERT_EQ (task_timers[0], 0) << ITERATE_consts::label_index << i;
			ASSERT_EQ (task_timers[1], 0) << ITERATE_consts::label_index << i;
			ASSERT_EQ (task_timers[2], 0) << ITERATE_consts::label_index << i;
			ASSERT_EQ (result, 110U) << ITERATE_consts::label_index << i;
			ASSERT_EQ (ITERATE_vars::reported_delta_time, 0)
				<< ITERATE_consts::label_index << i;
		}
	}
}

TEST (ITERATE, MULTI_DELAYED_OVERFLOW)
{
	const struct scheduler_task tasks[]
		= { { .handler = &ITERATE_funcs::dummy_handler, .millisecond = 110 },
			{ .handler = &ITERATE_funcs::dummy_handler_1, .millisecond = 177 },
			{ .handler = &ITERATE_funcs::dummy_handler_2,
			  .millisecond = 333 } };

	const unsigned int task_count
		= sizeof (tasks) / sizeof (struct scheduler_task);

	uint32_t task_timers[task_count] = { 0 };
	struct scheduler_context context = { .last_timestamp = 0,
										 .tasks = tasks,
										 .task_timers = task_timers,
										 .task_count = task_count };

	ITERATE_vars::reported_delta_time = 0;
	ITERATE_vars::reported_delta_time_1 = 0;
	ITERATE_vars::reported_delta_time_2 = 0;

	for (uint64_t i = OVERFLOW_START; i < OVERFLOW_END; i++) {
		uint32_t result = scheduler_iterate (&context, i);

		const uint32_t expected_timer = (OVERFLOW_START - i - 1) % 110U;
		const uint32_t expected_timer_1 = (OVERFLOW_START - i - 1) % 177U;
		const uint32_t expected_timer_2 = (OVERFLOW_START - i - 1) % 333U;

		uint32_t smallest_remaining = 110U - expected_timer;
		if (177U - expected_timer_1 < smallest_remaining)
			smallest_remaining = 177U - expected_timer_1;
		if (333U - expected_timer_2 < smallest_remaining)
			smallest_remaining = 333U - expected_timer_2;

		ASSERT_EQ (task_timers[0], expected_timer)
			<< ITERATE_consts::label_index << i;
		ASSERT_EQ (task_timers[1], expected_timer_1)
			<< ITERATE_consts::label_index << i;
		ASSERT_EQ (task_timers[2], expected_timer_2)
			<< ITERATE_consts::label_index << i;

		ASSERT_EQ (result, smallest_remaining)
			<< ITERATE_consts::label_index << i;

		if (expected_timer)
			ASSERT_EQ (ITERATE_vars::reported_delta_time, 0)
				<< ITERATE_consts::label_index << i;
		else
			ASSERT_EQ (ITERATE_vars::reported_delta_time, 110U)
				<< ITERATE_consts::label_index << i;

		if (expected_timer_1)
			ASSERT_EQ (ITERATE_vars::reported_delta_time_1, 0)
				<< ITERATE_consts::label_index << i;
		else
			ASSERT_EQ (ITERATE_vars::reported_delta_time_1, 177U)
				<< ITERATE_consts::label_index << i;

		if (expected_timer_2)
			ASSERT_EQ (ITERATE_vars::reported_delta_time_2, 0)
				<< ITERATE_consts::label_index << i;
		else
			ASSERT_EQ (ITERATE_vars::reported_delta_time_2, 333U)
				<< ITERATE_consts::label_index << i;

		ITERATE_vars::reported_delta_time = 0;
		ITERATE_vars::reported_delta_time_1 = 0;
		ITERATE_vars::reported_delta_time_2 = 0;
	}
}

#endif // TEST_UNIT
