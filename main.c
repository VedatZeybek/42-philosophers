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
	struct timeval	last_eat_time;
	pthread_t		thread;
	t_table			*table;
	int				philo_id;
	int				*data;
	int				left_fork;
	int				right_fork;
}	t_philo;

typedef struct s_table
{
	pthread_mutex_t	*forks;
	pthread_mutex_t print_mutex;
	t_philo			**philo;
	struct timeval	start_time;
	int				simulation_end;
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

long	time_diff_ms(struct timeval start, struct timeval end)
{
	return ((end.tv_sec - start.tv_sec) * 1000) + ((end.tv_usec - start.tv_usec) / 1000);
}

long get_timestamp(t_table *table)
{
    struct timeval current;

    gettimeofday(&current, NULL);
    return ((current.tv_sec - table->start_time.tv_sec) * 1000) + 
           ((current.tv_usec - table->start_time.tv_usec) / 1000);
}

void	safe_print(char *str, t_philo *philo)
{
	pthread_mutex_lock(&philo->table->print_mutex);
	if (!philo->table->simulation_end)
		printf("%ld %d %s\n", get_timestamp(philo->table), philo->philo_id, str);
	pthread_mutex_unlock(&philo->table->print_mutex);
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

void	philo_life_cycle(t_philo *philo, int left, int right)
{
	struct timeval	last_eat;
	int 			i;
	
	i = 0;
	while ((philo->table->cycle_count == -1 || i < philo->table->cycle_count) 
				&& !philo->table->simulation_end)
	{
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
		gettimeofday(&last_eat, NULL);
		philo->last_eat_time = last_eat;
		usleep(philo->table->time_to_eat);
		pthread_mutex_unlock(&philo->table->forks[left]);
		pthread_mutex_unlock(&philo->table->forks[right]);
		printf("Philo %d is sleeping.. \n", philo->philo_id);
		usleep(philo->table->time_to_sleep);
		printf("Philo %d is thinking...\n", philo->philo_id);
		i++;
	}
}


void	*philo_function(void* arg) 
{
	t_philo	*philo = (t_philo *)arg;
	int		i;
	int		left = philo->philo_id;
	int		right = (philo->philo_id + 1) % philo->table->philo_count;
	

	philo_life_cycle(philo, left, right);

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
