/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jteissie <jteissie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 13:52:12 by jteissie          #+#    #+#             */
/*   Updated: 2024/08/06 19:11:55 by jteissie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <unistd.h>
# include <pthread.h>
# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# define FALSE 0
# define TRUE 1
# define SUCCESS 0
# define PARSE_ERROR 1
# define PANIC 1
# define EXIT_SUCCESS 0
# define EXIT_FAILURE 1
# ifndef MAX_PHILO
#  define MAX_PHILO 200
# endif

typedef struct s_philo
{
	int				*death;
	long			start_time;
	int				number;
	int				time_to_sleep;
	int				time_to_eat;
	int				time_since_meal;
	int				meals_nb;
	int				think_time;
	int				left_fork;
	int				right_fork;
	pthread_mutex_t	*meal_lock;
	pthread_mutex_t	*print_stick;
	pthread_mutex_t	*death_lock;
	pthread_mutex_t	*forks;
}	t_philo;

typedef struct s_config
{
	int				death;
	int				meals_nb;
	int				philos_nb;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	long			start_time;
	pthread_mutex_t	*meal_lock;
	pthread_mutex_t	*print_stick;
	pthread_mutex_t	*death_lock;
	pthread_mutex_t	forks[MAX_PHILO];
}	t_config;

void		print_log(char *str, int fd);
void		throw_args_error(void);
int			simple_atoi(const char *nptr);
int			check_number(int ac, char **av);

void		destroy_mutexes(int philos_nb, t_config *config);
int			init_mutexes(int philos_nb, t_config *config);

long		get_start_time();
long		get_current_time(long start_time);
void		ft_usleep(t_philo *philo, int time_ms, long start_time);


void	*philo_routine(void *arg);

#endif
