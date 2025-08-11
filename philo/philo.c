#include "philosophers.h"

static void	arrange_forks(int *first, int *second, t_philo *philo)
{
	int	temp;

	if (philo->philo_id % 2 == 0)
	{
		temp = *first;
		*first = *second;
		*second = temp;
	}
}

static void	*philo_function(void* arg) 
{
	t_philo	*philo;
	int		first_fork;
	int		second_fork;
 
	philo = (t_philo *)arg;
	first_fork = philo->philo_id - 1;
	second_fork = (philo->philo_id) % philo->table->philo_count;
	arrange_forks(&first_fork, &second_fork, philo);
	//printf("p_id: %d, left fork %d, right fork: %d\n", philo->philo_id , first_fork , second_fork);
	philo_life_cycle(philo, first_fork, second_fork);
	return (NULL);
}

int	main(int argc, char **argv)
{
	int			i;
	t_table		*table;
	pthread_t	death_thread;

	if (!validate_arguments(argc, argv))
		return (1);
	table = fill_table_stats(argv);
	i = 0;
	while (i < table->philo_count)
	{
		pthread_create(&(table->philo[i]->thread), NULL, philo_function, table->philo[i]);
		i++;
	}
	pthread_create(&death_thread, NULL, death_checker, table);
	pthread_join(death_thread, NULL);
	i = 0;
	while (i < table->philo_count)
	{
		pthread_join(table->philo[i]->thread, NULL);
		i++;
	}
	cleanup_table(table);
}
