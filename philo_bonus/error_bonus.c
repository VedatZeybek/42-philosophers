/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzeybek <vzeybek@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 10:33:39 by vzeybek           #+#    #+#             */
/*   Updated: 2025/08/12 10:33:40 by vzeybek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

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
