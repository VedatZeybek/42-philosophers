#include "philosophers_bonus.h"

int	main(int argc, char **argv)
{
	t_table	*table;
	int		i;
	pid_t	*pids;

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
			exit(0);
		}
		else if (pids[i] < 0)
		{
			printf("Fork failed\n");
			return (1);
		}
		i++;
	}
	i = 0;
	while (i < table->philo_count)
	{
		waitpid(pids[i], 0, 0);
		i++;
	}
	sem_close(table->forks);
	sem_unlink("/forks");
	free(pids);
	return (0);
}