#include "philosophers_bonus.h"

int	get_death_value(t_philo *philo)
{
	int	result;

	sem_wait(philo->table->death_flag_sem);
	result = philo->table->death_flag;
	sem_post(philo->table->death_flag_sem);
	return (result);
}

void	print_message(t_philo *philo, char *message)
{
	if (get_death_value(philo) == 0)
	{
		sem_wait(philo->table->message);
		printf("%ld %d %s\n", get_timestamp(philo->table), 
		       philo->philo_id, message);
		sem_post(philo->table->message);
	}
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

void	cleanup_table(t_table *table)
{
	int	i;

	i = 0;
	sem_close(table->death);
	sem_close(table->message);
	sem_close(table->death_flag_sem);
	sem_close(table->forks);
	while (i < table->philo_count)
	{
		free(table->philo[i]);
		i++;
	}
	free(table->philo);
	free(table);
}

int	validate_arguments(int argc, char **argv)
{
	int	i;
	int	value;

	if (argc < 5 || argc > 6)
	{
		printf("Invalid Argument Usage.\n");
		return (0);
	}
	i = 1;
	while (i < argc)
	{
		if (!is_valid_number(argv[i]))
		{
			printf("'%s' is not a valid positive number\n", argv[i]);
			return (0);
		}
		value = ft_atoi(argv[i]);
		if (value <= 0)
		{
			printf("All arguments must be positive numbers\n");
			return (0);
		}
		i++;
	}
	value = ft_atoi(argv[1]);
	if (value < 1 || value > 200)
	{
		printf("Number of philosophers must be between 1 and 200\n");
		return (0);
	}
	return (1);
}
