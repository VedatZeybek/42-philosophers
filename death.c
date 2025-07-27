#include "philosophers.h"

void	*death_checker(void *arg)
{
	t_table	*table;
	struct	timeval	now;
	long	time_diff;
	int		i;

	table = (t_table *)(arg);
	usleep(10000);
	while (!table->simulation_end)
	{
		i = 0;
		while (i < table->philo_count)
		{
			gettimeofday(&now, NULL);
			pthread_mutex_lock(&table->philo[i]->last_eat_mutex);
			time_diff = time_diff_ms(table->philo[i]->last_eat_time, now);
			pthread_mutex_unlock(&table->philo[i]->last_eat_mutex);
			if (time_diff > table->time_to_die)
			{
				pthread_mutex_lock(&table->print_mutex);
				if (!table->simulation_end)
				{
					printf("%ld %d died\n", get_timestamp(table), table->philo[i]->philo_id);
					table->simulation_end = 1;
				}
				pthread_mutex_unlock(&table->print_mutex);
				return (NULL);
			}
			i++;
		}
		usleep(1000);
	}
	return (NULL);
}