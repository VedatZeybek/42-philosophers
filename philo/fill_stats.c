/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_stats.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzeybek <vzeybek@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 09:53:54 by vzeybek           #+#    #+#             */
/*   Updated: 2025/08/12 09:53:55 by vzeybek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	fill_philo_stats(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->philo_count)
	{
		pthread_mutex_init(&table->forks[i], NULL);
		table->philo[i] = malloc(sizeof(t_philo));
		pthread_mutex_init(&table->philo[i]->last_eat_mutex, NULL);
		pthread_mutex_init(&table->philo[i]->eat_count_mutex, NULL);
		table->philo[i]->philo_id = i + 1;
		table->philo[i]->table = table;
		table->philo[i]->last_eat_time = table->start_time;
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
	table->philo = malloc (sizeof(t_philo *) * count);
	table->forks = malloc(sizeof(pthread_mutex_t) * count);
	table->philo_count = count;
	table->simulation_end = 0;
	pthread_mutex_init(&table->print_mutex, NULL);
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
