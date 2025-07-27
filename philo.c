#include "philosophers.h"

void	*philo_function(void* arg) 
{
	t_philo	*philo;
	int		left;
	int		right;
	int		first_fork;
	int		second_fork;
	
	philo = (t_philo *)arg;
	left = philo->philo_id - 1;
	right = (philo->philo_id) % philo->table->philo_count;
	first_fork = left < right ? left : right;
	second_fork = left < right ? right : left;
	philo_life_cycle(philo, left, right);
	return (NULL);
}

int	main(int argc, char **argv)
{
	int			philo_count;
	int			i;
	t_table		*table;
	pthread_t	death_thread;

	philo_count = get_philo_count(argv);
	if (philo_count < 1 || philo_count > 200)
		return (0);
	table = fill_table_stats(philo_count, argv);
	i = 0;
	while (i < philo_count)
	{
		pthread_create(&(table->philo[i]->thread), NULL, philo_function, table->philo[i]);
		i++;
	}
	pthread_create(&death_thread, NULL, death_checker, table);
	pthread_join(death_thread, NULL);
	i = 0;
	while (i < philo_count)
	{
		pthread_join(table->philo[i]->thread, NULL);
		i++;
	}
	cleanup_table(table);
}
