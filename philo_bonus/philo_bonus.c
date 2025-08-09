#include "philosophers_bonus.h"
#include "signal.h"

int	main(int argc, char **argv)
{
	t_table		*table;
	int			i;
	pid_t		*pids;

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
	sem_wait(table->death);
	table->death_flag = 1;
	i = 0;
	while (i < table->philo_count)
	{
		kill(pids[i], SIGKILL);
		i++;
	}
	sem_close(table->forks);
	sem_unlink("/forks");
	sem_close(table->death);
	sem_unlink("/death");
	sem_close(table->message);
	sem_unlink("/message");
	free(pids);
	return (0);
}
