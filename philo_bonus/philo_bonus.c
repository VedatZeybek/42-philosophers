#include "philosophers_bonus.h"
#include "fcntl.h"
# include <sys/wait.h>

void	philo_process(t_philo *philo)
{
	int		death;

	death = 0;
	while (death < 5)
	{
		printf("%d %s\n", death, "eat");
		usleep(1000);
		death++;
	}
}


int	main(int argc, char **argv)
{
 
	t_philo *philo;
	int		count;
	int		i;

	i = 0;
	count = 2;
	while (i < count)
	{
		pid_t pid0 = fork();
		if (pid0 == 0)
		{
			philo_process(philo);
			exit(EXIT_SUCCESS);
		}
		i++;
	}
	wait(NULL);
}