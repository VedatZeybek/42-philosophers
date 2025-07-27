#include "philosophers.h"

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

void	cleanup_table(t_table *table)
{
    int i = 0;
    
    while (i < table->philo_count)
    {
        pthread_mutex_destroy(&table->philo[i]->last_eat_mutex);
        pthread_mutex_destroy(&table->forks[i]);
        free(table->philo[i]);
        i++;
    }
    pthread_mutex_destroy(&table->print_mutex);

    free(table->forks);
    free(table->philo);
    free(table);
}
