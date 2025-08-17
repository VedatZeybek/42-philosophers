/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_bonus.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzeybek <vzeybek@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 10:36:15 by vzeybek           #+#    #+#             */
/*   Updated: 2025/08/12 10:36:16 by vzeybek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_BONUS_H
# define  PHILOSOPHERS_BONUS_H

# include "stdlib.h"
# include "stdio.h"
# include <unistd.h> 
# include <sys/time.h>
# include "pthread.h"
# include "fcntl.h"
# include <sys/wait.h>
# include <semaphore.h>
# include <string.h>
# include "error_bonus.h"
# include "signal.h"

struct					s_table;
typedef struct s_table	t_table;

typedef struct s_philo
{
	struct timeval	last_eat_time;
	sem_t			*last_eat_sem;
	sem_t			*eat_count_sem;
	t_table			*table;
	int				philo_id;
	int				eat_count;
}	t_philo;

typedef struct s_table
{
	struct timeval	start_time;
	t_philo			**philo;
	sem_t			*forks;
	sem_t			*death;
	sem_t			*message;
	sem_t			*death_flag_sem;
	int				philo_count;
	int				time_to_eat;
	int				time_to_die;
	int				time_to_sleep;
	int				cycle_count;
	int				death_flag;
}	t_table;

char		*ft_itoa(int n);
char		*ft_strjoin(char const *s1, char const *s2);
int			ft_atoi(const char *str);
int			get_death_value(t_philo *philo);
int			validate_arguments(int argc, char **argv);
void		print_message(t_philo *philo, char *msg);
long		time_diff_ms(struct timeval start, struct timeval end);
long		get_timestamp(t_table *table);
int			philo_eat(t_philo *philo);
int			one_philo(t_philo *philo);
void		routine(t_philo *philo);
void		philo_process(t_philo *philo);
void		cleanup_table(t_table *table);
void		kill_all_remaining_philosophers(pid_t *pids, int count);
t_table		*fill_table_stats(char **argv);
void		cleanup_semaphores(t_table *table);

#endif
