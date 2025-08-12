/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzeybek <vzeybek@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 10:36:50 by vzeybek           #+#    #+#             */
/*   Updated: 2025/08/12 10:44:32 by vzeybek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

int	get_death_value(t_philo *philo)
{
	int	result;

	sem_wait(philo->table->death_flag_sem);
	result = philo->table->death_flag;
	sem_post(philo->table->death_flag_sem);
	return (result);
}

int	one_philo(t_philo *philo)
{
	if (philo->table->philo_count == 1)
	{
		sem_wait(philo->table->forks);
		print_message(philo, "has taken a fork");
		usleep(philo->table->time_to_die * 1000);
		usleep(10000);
		sem_post(philo->table->forks);
		if (get_death_value(philo))
			return (1);
	}
	return (0);
}

int	philo_eat(t_philo *philo)
{
	sem_wait(philo->table->forks);
	print_message(philo, "has taken a fork");
	sem_wait(philo->table->forks);
	print_message(philo, "has taken a fork");
	sem_wait(philo->last_eat_sem);
	gettimeofday(&philo->last_eat_time, NULL);
	sem_post(philo->last_eat_sem);
	sem_wait(philo->eat_count_sem);
	philo->eat_count++;
	sem_post(philo->eat_count_sem);
	print_message(philo, "is eating");
	usleep(1000 * philo->table->time_to_eat);
	sem_post(philo->table->forks);
	sem_post(philo->table->forks);
	sem_wait(philo->eat_count_sem);
	if (philo->table->cycle_count != -1
		&& philo->eat_count >= philo->table->cycle_count)
	{
		usleep(1000 * philo->table->time_to_eat);
		sem_wait(philo->table->message);
		sem_post(philo->eat_count_sem);
		return (1);
	}
	sem_post(philo->eat_count_sem);
	return (0);
}

void	routine(t_philo *philo)
{
	while (1)
	{
		if (philo_eat(philo))
			break ;
		if (get_death_value(philo))
			break ;
		print_message(philo, "is sleeping");
		if (get_death_value(philo))
			break ;
		usleep(1000 * philo->table->time_to_sleep);
		if (get_death_value(philo))
			break ;
		print_message(philo, "is thinking");
		if (philo->table->time_to_eat > philo->table->time_to_sleep)
		{
			usleep(500 + (philo->table->time_to_eat * 1000
					- philo->table->time_to_sleep * 1000));
		}
		else
			usleep(500);
		if (get_death_value(philo))
			break ;
	}
}
