/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzeybek <vzeybek@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 09:59:59 by vzeybek           #+#    #+#             */
/*   Updated: 2025/08/12 10:00:27 by vzeybek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define  PHILOSOPHERS_H

# include "stdlib.h"
# include "pthread.h"
# include "stdio.h"
# include <unistd.h> 
# include <sys/time.h>
# include "error.h"

struct					s_table;
typedef struct s_table	t_table;

typedef struct s_philo
{
	struct timeval	last_eat_time;
	pthread_mutex_t	last_eat_mutex;
	pthread_mutex_t	eat_count_mutex;
	pthread_t		thread;
	t_table			*table;
	int				philo_id;
	int				*data;
	int				eat_count;
}	t_philo;

typedef struct s_table
{
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_mutex;
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
int		validate_arguments(int argc, char **argv);
long	time_diff_ms(struct timeval start, struct timeval end);
long	get_timestamp(t_table *table);
void	safe_print(char *str, t_philo *philo);
void	cleanup_table(t_table *table);
void	philo_life_cycle(t_philo *philo, int first_fork, int second_fork);
void	*death_checker(void *arg);
t_table	*fill_table_stats(char **argv);

#endif