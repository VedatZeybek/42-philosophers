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
	pthread_mutex_t	*forks;
	t_philo			**philo;
	int				philo_count;
	int				time_to_eat;
	int				time_to_die;
	int				time_to_sleep;
	int				cycle_count;
}	t_table;

int	get_philo_count(char **argv)
{
	int	count;
	
	count = ft_atoi(argv[1]);
	return (count);
}

t_table	*fill_table_stats(int count, char **argv)
{
	t_table	*table;
	int		i;
	
	i = 0;
	table = malloc(sizeof(t_table));
	table->philo = malloc (sizeof(t_philo *) * count);
	table->forks = malloc(sizeof(pthread_mutex_t) * count);
	table->time_to_die = ft_atoi(argv[2]);
	table->time_to_eat = ft_atoi(argv[3]);
	table->time_to_sleep = ft_atoi(argv[4]);
	if (!argv[5])
		table->cycle_count = -1;
	else
		table->cycle_count = ft_atoi(argv[5]);
	while (i < count)
	{
		pthread_mutex_init(&table->forks[i], NULL);
		table->philo[i] = malloc(sizeof(t_philo));
		table->philo[i]->philo_id = i;
		table->philo[i]->left_fork = 0;
		table->philo[i]->right_fork = 0;
		table->philo[i]->table = table;
		table->philo_count = count;
		i++;
	}
	return (table);
}

void	philo_life_cycle(t_philo *philo)
{
	int		left = philo->philo_id;
	int		right = (philo->philo_id + 1) % philo->table->philo_count;
	if (philo->philo_id % 2 == 0)
	{
		pthread_mutex_lock(&philo->table->forks[left]);
		printf("Philo %d has took left fork...\n", philo->philo_id);
		pthread_mutex_lock(&philo->table->forks[right]);
		printf("Philo %d has took right fork...\n", philo->philo_id);
	}
	else
	{
		pthread_mutex_lock(&philo->table->forks[right]);
		printf("Philo %d has took right fork...\n", philo->philo_id);
		pthread_mutex_lock(&philo->table->forks[left]);
		printf("Philo %d has took left fork...\n", philo->philo_id);
	}
	printf("Philo %d is eating...\n", philo->philo_id);
	usleep(philo->table->time_to_eat);
	pthread_mutex_unlock(&philo->table->forks[left]);
	pthread_mutex_unlock(&philo->table->forks[right]);
	printf("Philo %d is sleeping.. \n", philo->philo_id);
	usleep(philo->table->time_to_sleep);
	printf("Philo %d is thinking...\n", philo->philo_id);
}

void	*philo_function(void* arg) 
{
	t_philo	*philo = (t_philo *)arg;
	int		i;

	if (philo->table->cycle_count == -1)
	{
		while (1)
		{
			philo_life_cycle(philo);
		}
	}
	else
	{
		i = 0;
		while (i < philo->table->cycle_count)
		{
			philo_life_cycle(philo);
			i++;
		}
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
	
	table = fill_table_stats(philo_count, argv);
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
