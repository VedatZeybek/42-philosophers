#include "philosophers_bonus.h"

static int	one_philo(t_philo *philo)
{
	if (philo->table->philo_count == 1)
	{
		sem_wait(philo->table->forks);
		print_message(philo, "has taken a fork");
		usleep(philo->table->time_to_die * 1000);
		usleep(100);
		sem_post(philo->table->forks);
		if (get_death_value(philo))
			return (1);
	}
	return (0);
}

static int	philo_eat(t_philo *philo)
{
	if (get_death_value(philo))
		return (1);
	sem_wait(philo->table->forks);
	print_message(philo, "has taken a fork");
	sem_wait(philo->table->forks);
	print_message(philo, "has taken a fork");
	sem_wait(philo->last_eat_sem);
	gettimeofday(&philo->last_eat_time, NULL);
	sem_post(philo->last_eat_sem);
	philo->eat_count++;
	print_message(philo, "is eating");
	usleep(1000 * philo->table->time_to_eat);
	sem_post(philo->table->forks);
	sem_post(philo->table->forks);
	if (philo->table->cycle_count != -1 && 
		philo->eat_count >= philo->table->cycle_count)
	{
		usleep(1000 * philo->table->time_to_eat);
		return (1);
	}
	return (0);
}

static void	*monitor_death(void *arg)
{
	t_philo			*philo;
	struct timeval	now;
	long			time_since_last_meal;

	philo = (t_philo *)arg;
	while (1)
	{
		sem_wait(philo->last_eat_sem);
		gettimeofday(&now, NULL);
		time_since_last_meal = time_diff_ms(philo->last_eat_time, now);
		sem_post(philo->last_eat_sem);
		if (time_since_last_meal > philo->table->time_to_die)
		{
			sem_wait(philo->table->message);
			printf("%ld %d died\n", get_timestamp(philo->table), philo->philo_id);
			sem_post(philo->table->message);
			sem_wait(philo->table->message);
			if (get_death_value(philo) == 0)
			{
				philo->table->death_flag = 1;
				break ;
			}
		}
		usleep(1000);
	}
	return (NULL);
}

void	philo_process(t_philo *philo)
{
	pthread_t	monitor;
	int			i;

	gettimeofday(&philo->last_eat_time, NULL);
	pthread_create(&monitor, NULL, monitor_death, philo);
	if (one_philo(philo))
	{
		pthread_join(monitor, NULL);
		return ;
	}
	while (1)
	{
		if (philo_eat(philo))
			break ;
		if (get_death_value(philo))
			break ;
		print_message(philo, "is sleeping");
		if (get_death_value(philo))
			break ;
		usleep(1000 * philo->table->time_to_sleep);
		if (get_death_value(philo))
			break ;
		print_message(philo, "is thinking");
		usleep(500 + (philo->table->time_to_eat * 1000 - philo->table->time_to_sleep * 1000));
		if (get_death_value(philo))
			break ;
		i++;
	}
	pthread_join(monitor, NULL);
	return ;
}
