#ifndef PHILOSOPHERS_BONUS_H
# define  PHILOSOPHERS_BONUS_H

#include "stdlib.h"
#include "stdio.h"
#include <unistd.h> 
#include <sys/time.h>
#include "fcntl.h"
#include <sys/wait.h>
#include <semaphore.h>

struct s_table;
typedef struct s_table t_table;

typedef struct s_philo
{
	struct timeval	last_eat_time;
	t_table			*table;
	int				philo_id;
	int				*data;
}	t_philo;

typedef struct s_table
{
	struct timeval	start_time;
	t_philo			**philo;
	sem_t			*forks;
	sem_t			*death;
	int				philo_count;
	int				time_to_eat;
	int				time_to_die;
	int				time_to_sleep;
	int				cycle_count;
}	t_table;


int		ft_atoi(const char *str);
int		validate_arguments(int argc, char **argv);
int		get_philo_count(char **argv);
long	time_diff_ms(struct timeval start, struct timeval end);
long	get_timestamp(t_table *table);
void	philo_process(t_philo *philo);
t_table	*fill_table_stats(char **argv);

#endif