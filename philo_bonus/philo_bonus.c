#include "philosophers_bonus.h"

static int	one_philo(t_philo *philo)
{
	if (philo->table->philo_count == 1)
	{
		sem_wait(philo->table->forks);
		printf("%d has taken a fork", philo->philo_id);
		usleep(philo->table->time_to_die * 1000);
		sem_post(philo->table->forks);
		return (1);
	}
	return (0);
}

static int	is_dead(t_philo *philo, long last_eat)
{
	long	current_time;

	current_time = get_timestamp(philo->table);
	if (current_time - last_eat > philo->table->time_to_die)
	{
		printf("%ld %d died\n", current_time, philo->philo_id);
		return (1);
	}
	return (0);
}

void	philo_process(t_philo *philo)
{
	int i = 0;

	if (one_philo(philo))
		return ;
	while ((philo->table->cycle_count == -1 || i < philo->table->cycle_count) 
		   && !philo->table->simulation_end)
	{
		sem_wait(philo->table->forks);
		printf("%ld %d has taken a fork\n", get_timestamp(philo->table), philo->philo_id);
        
		sem_wait(philo->table->forks);
		printf("%ld %d has taken a fork\n", get_timestamp(philo->table), philo->philo_id);
        printf("%ld %d %s\n", get_timestamp(philo->table), philo->philo_id, "is eating.");
		usleep(philo->table->time_to_eat * 1000);
		sem_post(philo->table->forks);
		sem_post(philo->table->forks);

        printf("%ld %d %s\n", get_timestamp(philo->table), philo->philo_id, "is sleeping.");
		usleep(philo->table->time_to_sleep * 1000); 
        printf("%ld %d %s\n", get_timestamp(philo->table), philo->philo_id, "is thinking."); 
		i++;
	}
}

int	main(int argc, char **argv)
{
 
	t_table *table;
	int		count;
	int		i;

	i = 0;
	count = get_philo_count(argv);
	table = fill_table_stats(count, argv);
	while (i < count)
	{
		pid_t pid0 = fork();
		if (pid0 == 0)
		{
			philo_process(table->philo[i]);
			exit(EXIT_SUCCESS);
		}
		i++;
	}
	
	sem_close(table->forks);
    sem_unlink("/forks");

	wait(NULL);
	wait(NULL);
	wait(NULL);
}