/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzeybek <vzeybek@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 10:00:48 by vzeybek           #+#    #+#             */
/*   Updated: 2025/08/20 15:03:18 by vzeybek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

long	time_diff_ms(struct timeval start, struct timeval end)
{
	return (((end.tv_sec - start.tv_sec) * 1000)
		+((end.tv_usec - start.tv_usec) / 1000));
}

long	get_timestamp(t_table *table)
{
	struct timeval	current;

	gettimeofday(&current, NULL);
	return (((current.tv_sec - table->start_time.tv_sec) * 1000)
		+ ((current.tv_usec - table->start_time.tv_usec) / 1000));
}

void	kill_all_remaining_philosophers(pid_t *pids, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		kill(pids[i], SIGKILL);
		i++;
	}
	i = 0;
	while (i < count)
	{
		if (pids[i] > 0)
		{
			waitpid(pids[i], NULL, WNOHANG);
		}
		i++;
	}
}

static void	cleanup_philosopher_semaphores(t_table *table, int i)
{
	char	*temp;
	char	*name;
	char	*eat_name;
	char	*eat_count_name;

	temp = ft_itoa(table->philo[i]->philo_id);
	name = ft_strjoin("/", temp);
	free(temp);
	sem_unlink(name);
	eat_name = ft_strjoin(name, "eat");
	sem_unlink(eat_name);
	eat_count_name = ft_strjoin(name, "eat");
	sem_unlink(eat_count_name);
	free(name);
	free(eat_name);
	free(eat_count_name);
	sem_close(table->philo[i]->last_eat_sem);
	sem_close(table->philo[i]->eat_count_sem);
}

void	cleanup_semaphores(t_table *table)
{
	int	i;

	sem_unlink("/forks");
	sem_unlink("/death");
	sem_unlink("/message");
	sem_unlink("/death_flag_sem");
	i = 0;
	while (i < table->philo_count)
	{
		cleanup_philosopher_semaphores(table, i);
		i++;
	}
}
