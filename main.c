#include "philosophers.h"

void	*death_checker(void *arg)
{
	t_table	*table;
	struct	timeval	now;
	long	time_diff;
	int		i;

	table = (t_table *)(arg);
	usleep(10000);
	while (!table->simulation_end)
	{
		i = 0;
		while (i < table->philo_count)
		{
			gettimeofday(&now, NULL);
			pthread_mutex_lock(&table->philo[i]->last_eat_mutex);
			time_diff = time_diff_ms(table->philo[i]->last_eat_time, now);
			pthread_mutex_unlock(&table->philo[i]->last_eat_mutex);
			if (time_diff > table->time_to_die)
			{
				pthread_mutex_lock(&table->print_mutex);
				if (!table->simulation_end)
				{
					printf("%ld %d died\n", get_timestamp(table), table->philo[i]->philo_id);
					table->simulation_end = 1;
				}
				pthread_mutex_unlock(&table->print_mutex);
				return (NULL);
			}
			i++;
		}
		usleep(1000);
	}
	return (NULL);
}

t_table	*fill_table_stats(int count, char **argv)
{
	t_table	*table;
	int		i;
	
	i = 0;
	table = malloc(sizeof(t_table));
	table->philo = malloc (sizeof(t_philo *) * count);
	table->forks = malloc(sizeof(pthread_mutex_t) * count);
	table->philo_count = count;
	table->simulation_end = 0;
	pthread_mutex_init(&table->print_mutex, NULL);
	table->time_to_die = ft_atoi(argv[2]);
	table->time_to_eat = ft_atoi(argv[3]);
	table->time_to_sleep = ft_atoi(argv[4]);
	gettimeofday(&table->start_time, NULL);
	if (!argv[5])
		table->cycle_count = -1;
	else
		table->cycle_count = ft_atoi(argv[5]);
	while (i < count)
	{
		pthread_mutex_init(&table->forks[i], NULL);
		table->philo[i] = malloc(sizeof(t_philo));
		pthread_mutex_init(&table->philo[i]->last_eat_mutex, NULL);
		table->philo[i]->philo_id = i + 1;
		table->philo[i]->left_fork = 0;
		table->philo[i]->right_fork = 0;
		table->philo[i]->table = table;
		table->philo[i]->last_eat_time = table->start_time;
		i++;
	}
	return (table);
}

void	philo_life_cycle(t_philo *philo, int first_fork, int second_fork)
{
	int i = 0;

	if (philo->table->philo_count == 1)
	{
		pthread_mutex_lock(&philo->table->forks[first_fork]);
		safe_print("has taken a fork", philo);
		usleep(philo->table->time_to_die * 1000);
		pthread_mutex_unlock(&philo->table->forks[first_fork]);
		return ;
	}
	while ((philo->table->cycle_count == -1 || i < philo->table->cycle_count) 
		   && !philo->table->simulation_end)
	{
		pthread_mutex_lock(&philo->table->forks[first_fork]);
		safe_print("has taken a fork", philo);
		if (philo->table->simulation_end)
		{
			pthread_mutex_unlock(&philo->table->forks[first_fork]);
			break;
		}
		pthread_mutex_lock(&philo->table->forks[second_fork]);
		safe_print("has taken a fork", philo);
		if (philo->table->simulation_end)
		{
			pthread_mutex_unlock(&philo->table->forks[first_fork]);
			pthread_mutex_unlock(&philo->table->forks[second_fork]);
			break;
		}
		pthread_mutex_lock(&philo->last_eat_mutex);
		gettimeofday(&philo->last_eat_time, NULL);
		pthread_mutex_unlock(&philo->last_eat_mutex);
		safe_print("is eating.", philo);
		usleep(philo->table->time_to_eat * 1000);
		pthread_mutex_unlock(&philo->table->forks[first_fork]);
		pthread_mutex_unlock(&philo->table->forks[second_fork]);
		if (philo->table->simulation_end)
			break;
		safe_print("is sleeping", philo);
		usleep(philo->table->time_to_sleep * 1000);
		safe_print("is thinking", philo);
		if (philo->table->cycle_count != -1)
			i++;
	}
}

void	*philo_function(void* arg) 
{
	t_philo	*philo;
	int		left;
	int		right;
	int		first_fork;
	int		second_fork;
	
	philo = (t_philo *)arg;
	left = philo->philo_id - 1;
	right = (philo->philo_id) % philo->table->philo_count;
	first_fork = left < right ? left : right;
	second_fork = left < right ? right : left;
	philo_life_cycle(philo, left, right);
	return (NULL);
}

int	main(int argc, char **argv)
{
	int			philo_count;
	int			i;
	t_table		*table;
	pthread_t	death_thread;

	philo_count = get_philo_count(argv);
	if (philo_count < 1 || philo_count > 200)
		return (0);
	table = fill_table_stats(philo_count, argv);
	i = 0;
	while (i < philo_count)
	{
		pthread_create(&(table->philo[i]->thread), NULL, philo_function, table->philo[i]);
		i++;
	}
	pthread_create(&death_thread, NULL, death_checker, table);
	pthread_join(death_thread, NULL);
	i = 0;
	while (i < philo_count)
	{
		pthread_join(table->philo[i]->thread, NULL);
		i++;
	}
}
