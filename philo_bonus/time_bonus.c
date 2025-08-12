/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzeybek <vzeybek@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 10:00:48 by vzeybek           #+#    #+#             */
/*   Updated: 2025/08/12 10:12:57 by vzeybek          ###   ########.fr       */
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
