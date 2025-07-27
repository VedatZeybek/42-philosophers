#include "philosophers.h"

void cleanup_table(t_table *table)
{
	int	i;
	i = 0;

	while (i < table->philo_count)
	{
		pthread_mutex_destroy(&table->forks[i]);
		free(table->philo[i]);
		i++;
	}
	pthread_mutex_destroy(&table->print_mutex);
	free(table->forks);
	free(table->philo);
	free(table);
}


int	get_philo_count(char **argv)
{
	int	count;

	count = ft_atoi(argv[1]);
	return (count);
}

void	safe_print(char *str, t_philo *philo)
{
	pthread_mutex_lock(&philo->table->print_mutex);
	if (!philo->table->simulation_end)
		printf("%ld %d %s\n", get_timestamp(philo->table), philo->philo_id, str);
	pthread_mutex_unlock(&philo->table->print_mutex);
}


// int	control_args(int argc, char **argv)
// {
// 	int	i;
// 	int	j;

// 	if (argc < 5 || argc > 6 || ft_atoi(argv[1]) > 20)
// 		return (0);
// 	i = 0;
// 	while (argv[i])
// 	{
// 		j = 0;
// 		while (argv[i][j])
// 		{
// 			if (argv[i][j] < '0' || argv[i][j] > '9')
// 				return (0);
// 			j++;
// 		}
// 		i++;
// 	}
// }