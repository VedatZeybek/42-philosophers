#include "philosophers_bonus.h"

static int	one_philo(t_philo *philo)
{
	if (philo->table->philo_count == 1)
	{
		sem_wait(philo->table->forks);
		printf("%d has taken a fork\n", philo->philo_id);
		usleep(philo->table->time_to_die * 1000);
		usleep(100);
		if (philo->table->death_flag)
			return (1);
	}
	return (0);
}

static int	philo_eat(t_philo *philo, long *last_meal_time)
{
	if (philo->table->death_flag)
		return (1);
	sem_wait(philo->table->forks);
	print_message(philo, "has taken a fork.");
	if (philo->table->death_flag)
	{
		sem_post(philo->table->forks);
		return (1);
	}
	sem_wait(philo->table->forks);
	print_message(philo, "has taken a fork.");
	*last_meal_time = get_timestamp(philo->table);
	print_message(philo, "is eating.");
	usleep(1000 * philo->table->time_to_eat);
	if (philo->table->death_flag)
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
	if (one_philo(philo))
		return ;
	last_meal_time = get_timestamp(philo->table);
	if (philo->philo_id % 2 == 0)
		usleep(1000);
	while (1)
	{
		if (philo_eat(philo, &last_meal_time))
			return ;
		print_message(philo, "is sleeping.");
		usleep(1000 * philo->table->time_to_sleep);
		if (philo->table->death_flag)
			return ;
		print_message(philo, "is thinking.");
		usleep(500);
		if (philo->table->death_flag)
			return ;
		i++;
	}
}
