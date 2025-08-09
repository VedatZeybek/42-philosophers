#include "philosophers_bonus.h"

void	print_message(t_philo *philo, char *message)
{
	if (philo->table->death_flag == 0)
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
