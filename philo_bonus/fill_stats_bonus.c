/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_stats_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzeybek <vzeybek@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 10:38:44 by vzeybek           #+#    #+#             */
/*   Updated: 2025/08/20 09:28:15 by vzeybek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

static void	init_philosopher_semaphores(t_philo *philo)
{
	char	*temp;
	char	*name;
	char	*eat_count_name;

	temp = ft_itoa(philo->philo_id);
	name = ft_strjoin("/", temp);
	free(temp);
	sem_unlink(name);
	philo->last_eat_sem = sem_open(name, O_CREAT, 0644, 1);
	eat_count_name = ft_strjoin(name, "eat");
	sem_unlink(eat_count_name);
	philo->eat_count_sem = sem_open(eat_count_name, O_CREAT, 0644, 1);
	free(name);
	free(eat_count_name);
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
		table->philo[i]->eat_count = 0;
		init_philosopher_semaphores(table->philo[i]);
		i++;
	}
}

static int	init_semaphores(t_table *table, int count)
{
	sem_unlink("/forks");
	table->forks = sem_open("/forks", O_CREAT, 0644, count);
	if (table->forks == SEM_FAILED)
		return (printf("sem_open failed\n"), exit(1), 0);
	sem_unlink("/death");
	table->death = sem_open("/death", O_CREAT, 0644, 0);
	if (table->death == SEM_FAILED)
		return (printf("sem_open failed\n"), exit(1), 0);
	sem_unlink("/message");
	table->message = sem_open("/message", O_CREAT, 0644, 1);
	if (table->message == SEM_FAILED)
		return (printf("sem_open failed\n"), exit(1), 0);
	sem_unlink("/death_flag_sem");
	table->death_flag_sem = sem_open("/death_flag_sem", O_CREAT, 0644, 1);
	if (table->death_flag_sem == SEM_FAILED)
		return (printf("sem_open failed\n"), exit(1), 0);
	return (1);
}

static void	set_table_values(t_table *table, char **argv, int count)
{
	table->philo_count = count;
	table->time_to_die = ft_atoi(argv[2]);
	table->time_to_eat = ft_atoi(argv[3]);
	table->time_to_sleep = ft_atoi(argv[4]);
	table->death_flag = 0;
	gettimeofday(&table->start_time, NULL);
	if (!argv[5])
		table->cycle_count = -1;
	else
		table->cycle_count = ft_atoi(argv[5]);
}

t_table	*fill_table_stats(char **argv)
{
	t_table	*table;
	int		count;

	count = ft_atoi(argv[1]);
	table = malloc(sizeof(t_table));
	if (!table)
		return (NULL);
	if (!init_semaphores(table, count))
		return (free(table), NULL);
	table->philo = malloc(sizeof(t_philo *) * count);
	if (!table->philo)
		return (free(table), NULL);
	set_table_values(table, argv, count);
	fill_philo_stats(table);
	return (table);
}
