#include "philosophers.h"

int	control_args(int argc, char **argv)
{
	int	i;
	int	j;

	if (argc < 5 || argc > 6 || ft_atoi(argv[1]) > 20)
		return (0);
	i = 0;
	while (argv[i])
	{
		j = 0;
		while (argv[i][j])
		{
			if (argv[i][j] < '0' || argv[i][j] > '9')
				return (0);
			j++;
		}
		i++;
	}
}


typedef struct s_philo
{
	int		philo_id;
	int		*data;
	int		left_fork;
	int		right_fork;
	t_table	*table;
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

void	fill_stats_table(t_table *table, int count)
{
	int	i;
	
	i = 0;
	table = malloc(sizeof(t_table));
	
	table->philo = malloc (sizeof(t_philo *) * count);
	table->forks = malloc(sizeof(int *));
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
	pthread_t thread1;
	pthread_t thread2;
	int		id_1 = 1;
	int		id_2 = 2;
	t_table	*table;

	philo_count = get_philo_count(argv);
	if (philo_count < 2 || philo_count > 20)
		return (0);

	fill_stats_table(table, philo_count);
	
	
	pthread_create(&thread1, NULL, philo_function, &table);
	pthread_create(&thread2, NULL, philo_function, &table);

	//pthread_detach(thread1);
    //printf("Thread1 detached - main won't wait\n");
    
    // 3. Thread2'yi join et
    // printf("Waiting for Thread2...\n");
    pthread_join(thread2, NULL);
    //printf("Thread2 result: %ld\n", (long)result);
    //sleep(1);
    
    //printf("Main thread finished\n");
}