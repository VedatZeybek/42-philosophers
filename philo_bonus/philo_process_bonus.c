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

static int	philo_eat(t_philo *philo)
{
	if (philo->table->death_flag)
		return (1);
	sem_wait(philo->table->forks);
	print_message(philo, "has taken a fork.");

	sem_wait(philo->table->forks);
	print_message(philo, "has taken a fork.");
	
	sem_wait(philo->last_eat_sem);
	gettimeofday(&philo->last_eat_time, NULL);
	sem_post(philo->last_eat_sem);

	print_message(philo, "is eating.");
	usleep(1000 * philo->table->time_to_eat);
	sem_post(philo->table->forks);
	sem_post(philo->table->forks);
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
			printf("%ld %d died.\n", get_timestamp(philo->table), philo->philo_id);
			sem_post(philo->table->death); // parent'a sinyal
			sem_post(philo->table->message);
			exit(1); // kendi process'ini bitir
		}
		usleep(1000); // 1 ms bekleme
	}
	return (NULL);
}


void	philo_process(t_philo *philo)
{
	pthread_t	monitor;

	pthread_create(&monitor, NULL, monitor_death, philo);
	pthread_detach(monitor);
	int		i;

	i = 0;
	if (one_philo(philo))
		return ;
	if (philo->philo_id % 2 == 0)
		usleep(1000);
	while (1)
	{
		if (philo_eat(philo))
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
