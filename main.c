#include "philosophers.h"

// int	control_args(int argc, char **argv)
// {
// 	int	i;
// 	int	j;

// 	if (argc < 5 || argc > 6 || ft_atoi(argv[1]) > 20)
// 		return (0);
// 	i = 0;
// 	while (argv[i])
// 	{
// 		j = 0;
// 		while (argv[i][j])
// 		{
// 			if (argv[i][j] < '0' || argv[i][j] > '9')
// 				return (0);
// 			j++;
// 		}
// 		i++;
// 	}
// }

struct s_table;
typedef struct s_table t_table;

typedef struct s_philo
{
	int			philo_id;
	int			*data;
	int			left_fork;
	int			right_fork;
	t_table		*table;
	pthread_t	thread;
}	t_philo;

typedef struct s_table
{
	int		*forks;
	t_philo	**philo;
	int		time_to_eat;
	int		time_to_think;
	int		time_to_sleep;
}	t_table;


int	get_philo_count(char **argv)
{
	int	count;
	
	count = ft_atoi(argv[1]);
	return (count);
}

t_table	*fill_stats_table(int count)
{
	t_table	*table;
	int		i;
	
	i = 0;
	table = malloc(sizeof(t_table));
	table->philo = malloc (sizeof(t_philo *) * count);
	table->forks = malloc(sizeof(int) * count);
	while (i < count)
	{
		table->philo[i] = malloc(sizeof(t_philo));
		table->philo[i]->philo_id = i;
		table->philo[i]->left_fork = 0;
		table->philo[i]->right_fork = 0;
		table->philo[i]->table = table;
		table->forks[i] = i;
		i++;
	}
	return (table);
}

void	*philo_function(void* arg) 
{
	t_philo	*philo = (t_philo *)arg;
	

	while (1)
	{
		printf("Philo %d has taken a fork...\n", philo->philo_id);

		printf("Philo %d is eating...\n", philo->philo_id);
		sleep(2);
		
		printf("Philo %d is thinking...\n", philo->philo_id);
		sleep(2);
		printf("Philo %d is sleeping.. \n", philo->philo_id);
	}
	return (NULL);
}

int	main(int argc, char **argv)
{
	
	int		philo_count;
	int		i;
	t_table	*table;

	
	philo_count = get_philo_count(argv);
	if (philo_count < 2 || philo_count > 20)
	return (0);
	
	table = fill_stats_table(philo_count);
	i = 0;
	while (i < philo_count)
	{
		pthread_create(&(table->philo[i]->thread), NULL, philo_function, table->philo[i]);
		i++;
	}
	i = 0;
	while (i < philo_count)
	{
		pthread_join(table->philo[i]->thread, NULL);
		i++;
	}

	//pthread_detach(thread1);
    //printf("Thread1 detached - main won't wait\n");
    // 3. Thread2'yi join et
    // printf("Waiting for Thread2...\n");
	//printf("Thread2 result: %ld\n", (long)result);
    //sleep(1);
    //printf("Main thread finished\n");
}