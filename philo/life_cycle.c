/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   life_cycle.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzeybek <vzeybek@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 09:54:01 by vzeybek           #+#    #+#             */
/*   Updated: 2025/09/26 11:29:00 by vzeybek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	one_philo(t_philo *philo, int first_fork)
{
	if (philo->table->philo_count == 1)
	{
		pthread_mutex_lock(&philo->table->forks[first_fork]);
		safe_print("has taken a fork", philo);
		usleep(philo->table->time_to_die * 1000);
		pthread_mutex_unlock(&philo->table->forks[first_fork]);
		return (1);
	}
	return (0);
}

static int	take_fork(t_philo *philo, int first_fork, int second_fork)
{
	pthread_mutex_lock(&philo->table->forks[first_fork]);
	safe_print("has taken a fork", philo);
	if (get_death_value(philo->table))
	{
		pthread_mutex_unlock(&philo->table->forks[first_fork]);
		return (1);
	}
	pthread_mutex_lock(&philo->table->forks[second_fork]);
	safe_print("has taken a fork", philo);
	if (get_death_value(philo->table))
	{
		pthread_mutex_unlock(&philo->table->forks[first_fork]);
		pthread_mutex_unlock(&philo->table->forks[second_fork]);
		return (1);
	}
	return (0);
}

static void	eat(t_philo *philo, int first_fork, int second_fork)
{
	pthread_mutex_lock(&philo->last_eat_mutex);
	gettimeofday(&philo->last_eat_time, NULL);
	pthread_mutex_unlock(&philo->last_eat_mutex);
	safe_print("is eating", philo);
	usleep(philo->table->time_to_eat * 1000);
	pthread_mutex_unlock(&philo->table->forks[first_fork]);
	pthread_mutex_unlock(&philo->table->forks[second_fork]);
	pthread_mutex_lock(&philo->eat_count_mutex);
	philo->eat_count++;
	pthread_mutex_unlock(&philo->eat_count_mutex);
}

static void	sleep_think(t_philo *philo)
{
	safe_print("is sleeping", philo);
	usleep(philo->table->time_to_sleep * 1000);
	safe_print("is thinking", philo);
	if (philo->table->time_to_eat > philo->table->time_to_sleep)
	{
		usleep(500 + (philo->table->time_to_eat * 1000
				- philo->table->time_to_sleep * 1000));
	}
	else
		usleep(500);
}

void	philo_life_cycle(t_philo *philo, int first_fork, int second_fork)
{
	int	eat_count;

	eat_count = 0;
	if (one_philo(philo, first_fork))
		return ;
	if (philo->philo_id % 2 == 0)
		usleep(500);
	while (!get_death_value(philo->table))
	{
		if (take_fork(philo, first_fork, second_fork))
			break ;
		eat(philo, first_fork, second_fork);
		pthread_mutex_lock(&philo->eat_count_mutex);
		eat_count = philo->eat_count;
		pthread_mutex_unlock(&philo->eat_count_mutex);
		pthread_mutex_lock(&philo->eat_count_mutex);
		if (philo->table->cycle_count != -1
			&& eat_count >= philo->table->cycle_count)
		{
			pthread_mutex_unlock(&philo->eat_count_mutex);
			break ;
		}
		pthread_mutex_unlock(&philo->eat_count_mutex);
		sleep_think(philo);
	}
}
