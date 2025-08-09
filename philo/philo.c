#include "philosophers.h"

static void	arrange_forks(int *first, int *second, int right, int left)
{
	if (right < left)
	{
		*first = right;
		*second = left;
	}
	else
	{
		*first = left;
		*second = right;
	}
}

static void	*philo_function(void* arg) 
{
	t_philo	*philo;
	int		left;
	int		right;
	int		first_fork;
	int		second_fork;
	int		temp;
 
	philo = (t_philo *)arg;
	right = philo->philo_id - 1;
	left = (philo->philo_id) % philo->table->philo_count;
	arrange_forks(&first_fork, &second_fork, right, left);
	if (philo->philo_id == philo->table->philo_count)
	{
		temp = first_fork;
		first_fork = second_fork;
		second_fork = temp;
	}
	// if (philo->philo_id == philo->table->philo_count
	// 	&& philo->table->philo_count % 2 == 1)
	// {
	// 	temp = first_fork;
	// 	first_fork = second_fork;
	// 	second_fork = temp;
	// }
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
