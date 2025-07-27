#include "philosophers_bonus.h"

int	get_philo_count(char **argv)
{
	int	count;

	count = ft_atoi(argv[1]);
	return (count);
}

static void	fill_philo_stats(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->philo_count)
	{
		table->philo[i] = malloc(sizeof(t_philo));
		table->philo[i]->philo_id = i + 1;
		table->philo[i]->table = table;
		table->philo[i]->last_eat_time = table->start_time;
		i++;
	}
}

t_table	*fill_table_stats(int count, char **argv)
{
	t_table	*table;

	table = malloc(sizeof(t_table));
	table->forks = sem_open("/forks", O_CREAT, 0644, count);
	if (table->forks == SEM_FAILED) {
		perror("sem_open failed");
		exit(1);
	}
	table->dining_room = sem_open("/dining_room", O_CREAT, 0644, count - 1);
	if (table->dining_room == SEM_FAILED)
	{
		perror("sem_open dining_room failed");
		exit(1);
	}
	table->philo = malloc (sizeof(t_philo *) * count);
	table->philo_count = count;
	table->simulation_end = 0;
	table->time_to_die = ft_atoi(argv[2]);
	table->time_to_eat = ft_atoi(argv[3]);
	table->time_to_sleep = ft_atoi(argv[4]);
	gettimeofday(&table->start_time, NULL);
	if (!argv[5])
		table->cycle_count = -1;
	else
		table->cycle_count = ft_atoi(argv[5]);
	fill_philo_stats(table);
	return (table);
}
