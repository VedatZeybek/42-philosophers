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
	int	i;

	i = 0;
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
	ree(table);
}

int	is_valid_number(char *str)
{
	int	i;

	i = 0;
	if (!str || str[0] == '\0')
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	validate_arguments(int argc, char **argv)
{
	int	i;
	int	value;

	if (argc < 5 || argc > 6)
	{
		printf("Invalid Argument Number.\n");
		return (0);
	}
	i = 1;
	while (i < argc)
	{
		if (!is_valid_number(argv[i]))
		{
			printf("Error: '%s' is not a valid.\n", argv[i]);
			return (0);
		}
		value = ft_atoi(argv[i]);
		if (value <= 0)
		{
			printf("Error: All arguments must be positive numbers.\n");
			return (0);
		}
		i++;
	}
	value = ft_atoi(argv[1]);
	if (value < 1 || value > 200)
	{
		printf("Error: Number of philosophers must be between 1 and 200\n");
		return (0);
	}
	return (1);
}
