#include "philosophers.h"

static int	all_philos_finished_eat(t_table *table)
{
	int	count;
	int	i;

	i = 0;
	count = 0;
	while (i < table->philo_count)
	{
		pthread_mutex_lock(&table->philo[i]->eat_count_mutex);
		if (table->philo[i]->eat_count == table->cycle_count)
			count++;
		pthread_mutex_unlock(&table->philo[i]->eat_count_mutex);
		i++;
	}
	if (count == table->philo_count)
		return (1);
	return (0);
}

static int	is_dead(t_table *table, int i)
{
	struct	timeval	now;
	long	time_diff;

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
		return (1);
	}
	return (0);
}

static void	death_cehcker_loop(t_table *table)
{
	int		i;

	i = 0;
	while (i < table->philo_count)
	{
		pthread_mutex_lock(&table->philo[i]->eat_count_mutex);
		if (table->philo[i]->eat_count >= table->cycle_count
				&& table->cycle_count != -1)
		{
			pthread_mutex_unlock(&table->philo[i]->eat_count_mutex);
			i++;
			continue ;
		}
		pthread_mutex_unlock(&table->philo[i]->eat_count_mutex);
		if (is_dead(table, i))
			return ;
		i++;
	}
}

void	*death_checker(void *arg)
{
	t_table	*table;

	table = (t_table *)(arg);
	usleep(100);
	while (!table->simulation_end)
	{
		death_cehcker_loop(table);
		if (all_philos_finished_eat(table) && !table->simulation_end)
		{
			table->simulation_end = 1;
			return (NULL);
		}
		usleep(100);
	}
	return (NULL);
}
