#include "philosophers_bonus.h"
#include "signal.h"

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

int	main(int argc, char **argv)
{
	t_table		*table;
	pid_t		*pids;
	int			i;

	i = 0;
	if (!validate_arguments(argc, argv))
		exit(EXIT_FAILURE);
	table = fill_table_stats(argv);
	pids = malloc(sizeof(pid_t) * table->philo_count);
	if (!pids)
		return (1);
	while (i < table->philo_count)
	{
		pids[i] = fork();
		if (pids[i] == 0)
		{
			philo_process(table->philo[i]);
			int	j = 0;
			while (j < table->philo_count)
			{
				sem_close(table->philo[j]->last_eat_sem);
				sem_close(table->philo[j]->eat_count_sem);
				j++;
			}			
			free(pids);
			cleanup_table(table);
			exit(1);
		}
		else if (pids[i] < 0)
		{
			printf("Fork failed\n");
			return (1);
		}
		i++;
	}
	int status;
	pid_t pid;
	int someone_died = 0;
	while (!someone_died)
	{
		pid = waitpid(-1, &status, 0);
		if (pid > 0) 
		{
			if (WIFEXITED(status))
			{
				int exit_code = WEXITSTATUS(status);
				if (exit_code == 1) 
				{
					someone_died = 1;
					kill_all_remaining_philosophers(pids, table->philo_count);
					break ;
				}
			}
		}
	}
	table->death_flag = 1;
	i = 0;
	sem_unlink("/forks");
	sem_unlink("/death");
	sem_unlink("/message");
	sem_unlink("/death_flag_sem");
	i = 0;
	while (i < table->philo_count)
	{
		char *temp = ft_itoa(table->philo[i]->philo_id);
		char *name = ft_strjoin("/", temp);
		free(temp);
		sem_unlink(name);
		char *eat_name = ft_strjoin(name, "eat");
		sem_unlink(eat_name);
		free(name);
		free(eat_name);
		sem_close(table->philo[i]->last_eat_sem);
		sem_close(table->philo[i]->eat_count_sem);
		i++;
	}
	cleanup_table(table);
	free(pids);
	return (0);
}
