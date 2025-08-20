/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_process_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzeybek <vzeybek@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 10:35:56 by vzeybek           #+#    #+#             */
/*   Updated: 2025/08/20 15:27:43 by vzeybek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

static int	check_philo_death(t_philo *philo)
{
	struct timeval	now;
	long			time_since_last_meal;

	sem_wait(philo->last_eat_sem);
	gettimeofday(&now, NULL);
	time_since_last_meal = time_diff_ms(philo->last_eat_time, now);
	sem_post(philo->last_eat_sem);
	if (time_since_last_meal > philo->table->time_to_die)
	{
		sem_wait(philo->table->message);
		printf("%ld %d died\n", get_timestamp(philo->table), philo->philo_id);
		sem_post(philo->table->message);
		sem_wait(philo->table->message);
		if (get_death_value(philo) == 0)
		{
			sem_wait(philo->table->death_flag_sem);
			philo->table->death_flag = 1;
			sem_post(philo->table->death_flag_sem);
			return (1);
		}
	}
	return (0);
}

static int	check_eat_count(t_philo *philo)
{
	int	should_exit;

	should_exit = 0;
	sem_wait(philo->eat_count_sem);
	if (philo->table->cycle_count != -1
		&& philo->eat_count >= philo->table->cycle_count)
		should_exit = 1;
	sem_post(philo->eat_count_sem);
	return (should_exit);
}

static void	*monitor_death(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (1)
	{
		if (check_philo_death(philo))
			break ;
		if (check_eat_count(philo))
			break ;
		usleep(1000);
	}
	return (NULL);
}

void	philo_process(t_philo *philo)
{
	pthread_t	monitor;

	gettimeofday(&philo->last_eat_time, NULL);
	pthread_create(&monitor, NULL, monitor_death, philo);
	if (one_philo(philo))
	{
		pthread_join(monitor, NULL);
		return ;
	}
	if (philo->philo_id % 2 == 0)
		usleep(10000);
	routine(philo);
	pthread_join(monitor, NULL);
	return ;
}
