#include "philosophers_bonus.h"

static int	is_dead(t_philo *philo, long last_eat)
{
	long	current_time;

	current_time = get_timestamp(philo->table);
	if (current_time - last_eat > philo->table->time_to_die)
	{
		sem_post(philo->table->death);
		printf("%ld %d died\n", current_time, philo->philo_id);
		return (1);
	}
	return (0);
}

static int	one_philo(t_philo *philo, long last_meal_time)
{
	if (philo->table->philo_count == 1)
	{
		sem_wait(philo->table->forks);
		printf("%d has taken a fork\n", philo->philo_id);
		usleep(philo->table->time_to_die * 1000);
		usleep(100);
		is_dead(philo, last_meal_time);
		return (1);
	}
	return (0);
}

static int	safe_sleep(t_philo *philo, long sleep_duration, long last_meal_time)
{
	long sleep_start;
	long elapsed;

	sleep_start = get_timestamp(philo->table);
	while (1)
	{
		if (is_dead(philo, last_meal_time))
			return (1);
		elapsed = get_timestamp(philo->table) - sleep_start;
		if (elapsed >= sleep_duration)
			break ;
		usleep(1000);
	}
	return (0);
}

static int	philo_eat(t_philo *philo, long *last_meal_time)
{
	if (is_dead(philo, *last_meal_time))
		return (1);
	sem_wait(philo->table->forks);
	printf("%ld %d has taken a fork\n", get_timestamp(philo->table), philo->philo_id);
	if (is_dead(philo, *last_meal_time))
	{
		sem_post(philo->table->forks);
		return (1);
	}
	sem_wait(philo->table->forks);
	printf("%ld %d has taken a fork\n", get_timestamp(philo->table), philo->philo_id);
	*last_meal_time = get_timestamp(philo->table);
	printf("%ld %d is eating\n", get_timestamp(philo->table), philo->philo_id);
	if (safe_sleep(philo, philo->table->time_to_eat, *last_meal_time))
	{
		sem_post(philo->table->forks);
		sem_post(philo->table->forks);
		return (1);
	}
	sem_post(philo->table->forks);
	sem_post(philo->table->forks);
	return (0);
}

void	philo_process(t_philo *philo)
{
	int		i;
	long	last_meal_time;

	i = 0;
	if (one_philo(philo, get_timestamp(philo->table)))
		return ;
	last_meal_time = get_timestamp(philo->table);
	while (1)
	{
		if (philo_eat(philo, &last_meal_time))
			return ;
		printf("%ld %d is sleeping\n", get_timestamp(philo->table), philo->philo_id);
		if (safe_sleep(philo, philo->table->time_to_sleep, last_meal_time))
			return ;
		printf("%ld %d is thinking\n", get_timestamp(philo->table), philo->philo_id);
		i++;
	}
}
