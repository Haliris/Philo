/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jteissie <jteissie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 13:52:12 by jteissie          #+#    #+#             */
/*   Updated: 2024/08/05 18:00:14 by jteissie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <unistd.h>
# include <pthread.h>
# include <string.h>
# include <stdio.h>
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
	int				dead;
	int				time_to_sleep;
	int				time_to_eat;
	int				eating;
	int				meals_nb;
	int				has_fork;
	int				think_time;
	int				left_fork;
	int				right_fork;
}	t_philo;

void	print_log(char *str, int fd);
void	throw_args_error(void);
int		simple_atoi(const char *nptr);
int		check_number(int ac, char **av);

void	destroy_mutexes(int philos_nb, pthread_mutex_t forks[]);
int		init_mutexes(int philos_nb, pthread_mutex_t forks[]);


void	routine(t_philo *config);
void	monitor_routine(int deat_time);
#endif
