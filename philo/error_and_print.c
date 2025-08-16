/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_and_print.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzeybek <vzeybek@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 09:53:50 by vzeybek           #+#    #+#             */
/*   Updated: 2025/08/16 13:33:09 by vzeybek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	safe_print(char *str, t_philo *philo)
{
	pthread_mutex_lock(&philo->table->print_mutex);
	pthread_mutex_lock(&philo->table->simulation_end_mutex);
	if (!philo->table->simulation_end)
		printf("%ld %d %s\n", get_timestamp(philo->table), philo->philo_id, str);
	pthread_mutex_unlock(&philo->table->print_mutex);
	pthread_mutex_unlock(&philo->table->simulation_end_mutex);
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
	pthread_mutex_destroy(&table->simulation_end_mutex);
	free(table->forks);
	free(table->philo);
	free(table);
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

static int	control_number(int argc, char **argv)
{
	int	i;
	int	value;

	i = 1;
	while (i < argc)
	{
		if (!is_valid_number(argv[i]))
		{
			printf(ERR_NUM);
			return (0);
		}
		value = ft_atoi(argv[i]);
		if (value <= 0)
		{
			printf(ERR_NEG);
			return (0);
		}
		i++;
	}
	return (1);
}

int	validate_arguments(int argc, char **argv)
{
	int	value;

	if (argc < 5 || argc > 6)
	{
		printf(ERR_INPUT);
		return (0);
	}
	if (!control_number(argc, argv))
		return (0);
	value = ft_atoi(argv[1]);
	if (value < 1 || value > 200)
	{
		printf(ERR_PHI);
		return (0);
	}
	return (1);
}
