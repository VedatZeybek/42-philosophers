#include "philosophers_bonus.h"

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
		sem_unlink("/last_eat_sem");
		table->philo[i]->last_eat_sem = sem_open("/last_eat_sem", O_CREAT, 0644, 1);
		table->philo[i]->eat_count = 0;
		i++;
	}
}

t_table	*fill_table_stats(char **argv)
{
	t_table	*table;
	int		count;

	count = ft_atoi(argv[1]);
	table = malloc(sizeof(t_table));
	sem_unlink("/forks");
	table->forks = sem_open("/forks", O_CREAT , 0644, count);
	if (table->forks == SEM_FAILED) 
	{
		printf("sem_open failed");
		exit(1);
	}
	sem_unlink("/death");
	table->death = sem_open("/death", O_CREAT, 0644, 0);
	sem_unlink("/message");
	table->message = sem_open("/message", O_CREAT, 0644, 1);
	table->philo = malloc (sizeof(t_philo *) * count);
	table->philo_count = count;
	table->time_to_die = ft_atoi(argv[2]);
	table->time_to_eat = ft_atoi(argv[3]);
	table->time_to_sleep = ft_atoi(argv[4]);
	table->death_flag = 0;
	sem_unlink("/death_flag_sem");
	table->death_flag_sem = sem_open("/death_flag_sem", O_CREAT, 0644, 1);
	gettimeofday(&table->start_time, NULL);
	if (!argv[5])
		table->cycle_count = -1;
	else
		table->cycle_count = ft_atoi(argv[5]);
	fill_philo_stats(table);
	return (table);
}
