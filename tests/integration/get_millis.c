#include <stdint.h>
#include <time.h>

uint64_t
get_millis (void)
{
	struct timespec tms = { 0 };
	timespec_get (&tms, TIME_UTC);
	return (tms.tv_sec * 1000ULL) + (tms.tv_nsec / 1000000ULL);
}
