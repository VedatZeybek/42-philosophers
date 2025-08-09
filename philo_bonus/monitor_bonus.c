// #include "philosophers_bonus.h"

// static void	*monitor_death(void *arg)
// {
// 	t_table			*table;
// 	int				i;
// 	struct timeval	now;
// 	long			time_since_last_meal;

// 	table = (t_table *)arg;
// 	while (table->death_flag == 0)
// 	{
// 		i = 0;
// 		while (i < table->philo_count && table->death_flag == 0)
// 		{
// 			sem_wait(table->philo[i]->last_eat_sem);
// 			gettimeofday(&now, NULL);
// 			time_since_last_meal = time_diff_ms(table->philo[i]->last_eat_time, now);
// 			sem_post(table->philo[i]->last_eat_sem);
// 			printf("time diff: %ld\n", time_since_last_meal);
// 			if (time_since_last_meal > table->time_to_die)
// 			{
// 				sem_wait(table->message);
// 				printf("%ld %d died.\n", get_timestamp(table), table->philo[i]->philo_id);
// 				table->death_flag = 1;
// 				sem_post(table->death);
// 				sem_post(table->message);
// 				return (NULL);
// 			}
// 			i++;
// 		}
// 		usleep(10000);
// 	}
// 	return (NULL);
// }

// pthread_t	start_monitor(t_table *table)
// {
// 	pthread_t	monitor_thread;

// 	pthread_create(&monitor_thread, NULL, monitor_death, table);
// 	return (monitor_thread);
// }
