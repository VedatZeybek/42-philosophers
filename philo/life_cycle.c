#include "philosophers.h"

static int	one_philo(t_philo *philo, int first_fork)
{
	if (philo->table->philo_count == 1)
	{
		pthread_mutex_lock(&philo->table->forks[first_fork]);
		safe_print("has taken a fork", philo);
		usleep(philo->table->time_to_die * 1000);
		pthread_mutex_unlock(&philo->table->forks[first_fork]);
		return (1);
	}
	return (0);
}

static int	take_fork(t_philo *philo, int first_fork, int second_fork)
{
	pthread_mutex_lock(&philo->table->forks[first_fork]);
	safe_print("has taken a fork", philo);
	if (philo->table->simulation_end)
	{
		pthread_mutex_unlock(&philo->table->forks[first_fork]);
		return (1);
	}
	pthread_mutex_lock(&philo->table->forks[second_fork]);
	safe_print("has taken a fork", philo);
	if (philo->table->simulation_end)
	{
		pthread_mutex_unlock(&philo->table->forks[first_fork]);
		pthread_mutex_unlock(&philo->table->forks[second_fork]);
		return (1);
	}
	return (0);
}

static void	eat(t_philo *philo, int first_fork, int second_fork)
{
	pthread_mutex_lock(&philo->last_eat_mutex);
	gettimeofday(&philo->last_eat_time, NULL);
	pthread_mutex_unlock(&philo->last_eat_mutex);
	safe_print("is eating.", philo);
	usleep(philo->table->time_to_eat * 1000);
	pthread_mutex_unlock(&philo->table->forks[first_fork]);
	pthread_mutex_unlock(&philo->table->forks[second_fork]);
}

static void	sleep_think(t_philo *philo)
{
	safe_print("is sleeping", philo);
	usleep(philo->table->time_to_sleep * 1000);
	safe_print("is thinking", philo);
}

void	philo_life_cycle(t_philo *philo, int first_fork, int second_fork)
{
	int i = 0;

	if (one_philo(philo, first_fork))
		return ;
	usleep(100 * philo->philo_id);
	while ((philo->table->cycle_count == -1 || i < philo->table->cycle_count) 
		   && !philo->table->simulation_end)
	{
		if (take_fork(philo, first_fork, second_fork))
			break ;
		eat(philo, first_fork, second_fork);
		if (philo->table->simulation_end)
			break ;
		sleep_think(philo);
		usleep(10);
		if (philo->table->simulation_end)
			break ;
		if (philo->table->cycle_count != -1)
			i++;
	}
}
