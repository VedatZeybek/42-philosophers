#include "philosophers_bonus.h"

int	main(int argc, char **argv)
{
	t_table	*table;
	int		count;
	int		i;
	pid_t	*pids;
	pid_t	dead_pid;

	i = 0;
	count = get_philo_count(argv);
	table = fill_table_stats(count, argv);
	pids = malloc(sizeof(pid_t) * count);
	if (!pids)
		return (1);
	while (i < count)
	{
		pids[i] = fork();
		if (pids[i] == 0)
		{
			philo_process(table->philo[i]);
			exit(0);
		}
		else if (pids[i] < 0)
		{
			printf("Fork failed\n");
			return (1);
		}
		i++;
	}
	dead_pid = waitpid(-1, NULL, 0);
	i = 0;
	while (i < count)
	{
		if (pids[i] != dead_pid)
			kill(pids[i], 9);
		i++;
	}
	i = 0;
	while (i < count)
	{
		if (pids[i] != dead_pid)
			waitpid(pids[i], NULL, 0);
		i++;
	}
	sem_close(table->forks);
	sem_unlink("/forks");
	free(pids);
	return (0);
}