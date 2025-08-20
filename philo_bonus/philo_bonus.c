/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzeybek <vzeybek@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 10:35:52 by vzeybek           #+#    #+#             */
/*   Updated: 2025/08/20 15:03:55 by vzeybek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

static void	cleanup_child_process(t_table *table, pid_t *pids)
{
	int	j;

	j = 0;
	while (j < table->philo_count)
	{
		sem_close(table->philo[j]->last_eat_sem);
		sem_close(table->philo[j]->eat_count_sem);
		j++;
	}
	free(pids);
	cleanup_table(table);
}

static int	create_philosopher_processes(t_table *table, pid_t *pids)
{
	int	i;

	i = 0;
	while (i < table->philo_count)
	{
		pids[i] = fork();
		if (pids[i] == 0)
		{
			philo_process(table->philo[i]);
			cleanup_child_process(table, pids);
			exit(1);
		}
		else if (pids[i] < 0)
		{
			printf("Fork failed\n");
			return (1);
		}
		i++;
	}
	return (0);
}

static int	wait_for_philosophers(pid_t *pids, t_table *table)
{
	int		status;
	pid_t	pid;
	int		someone_died;

	someone_died = 0;
	while (!someone_died)
	{
		pid = waitpid(-1, &status, WNOHANG);
		if (pid > 0)
		{
			if (WIFEXITED(status))
			{
				if (WEXITSTATUS(status) == 1)
				{
					someone_died = 1;
					kill_all_remaining_philosophers(pids, table->philo_count);
					break ;
				}
			}
		}
	}
	return (someone_died);
}

int	main(int argc, char **argv)
{
	t_table	*table;
	pid_t	*pids;

	if (!validate_arguments(argc, argv))
		exit(EXIT_FAILURE);
	table = fill_table_stats(argv);
	pids = malloc(sizeof(pid_t) * table->philo_count);
	if (!pids)
		return (1);
	if (create_philosopher_processes(table, pids))
		return (1);
	wait_for_philosophers(pids, table);
	table->death_flag = 1;
	cleanup_semaphores(table);
	cleanup_table(table);
	free(pids);
	return (0);
}
