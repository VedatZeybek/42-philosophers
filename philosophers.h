#ifndef PHILOSOPHERS_H
# define  PHILOSOPHERS_H


#include "stdlib.h"
#include "pthread.h"
#include "stdio.h"
#include <unistd.h> 
#include <sys/time.h>


struct s_table;
typedef struct s_table t_table;

typedef struct s_philo
{
	struct timeval	last_eat_time;
	pthread_mutex_t last_eat_mutex;
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


int		ft_atoi(const char *str);
void	safe_print(char *str, t_philo *philo);
int		get_philo_count(char **argv);
long	time_diff_ms(struct timeval start, struct timeval end);
long	get_timestamp(t_table *table);
void	 cleanup_table(t_table *table);



#endif