#include "philosophers_bonus.h"

long	time_diff_ms(struct timeval start, struct timeval end)
{
	return ((end.tv_sec - start.tv_sec) * 1000) + ((end.tv_usec - start.tv_usec) / 1000);
}

long get_timestamp(t_table *table)
{
	struct timeval current;

	gettimeofday(&current, NULL);
	return ((current.tv_sec - table->start_time.tv_sec) * 1000) + 
			((current.tv_usec - table->start_time.tv_usec) / 1000);
}
