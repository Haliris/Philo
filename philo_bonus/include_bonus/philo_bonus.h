/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 17:40:43 by jteissie          #+#    #+#             */
/*   Updated: 2024/08/12 22:57:00 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H
# include <unistd.h>
# include <pthread.h>
# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <semaphore.h>
# include <signal.h>
# define FALSE 0
# define TRUE 1
# define SUCCESS 0
# define PARSE_ERROR 1
# define PANIC 1
# define UNLOCKED 0
# define LOCKED 1
# define EXIT_SUCCESS 0
# define EXIT_FAILURE 1
# define EXIT_DEATH 3
# ifndef MAX_PHILO
#  define MAX_PHILO 200
# endif

typedef enum e_sem_error
{
	OK,
	FORK_ERR,
	PRINT_ERR,
	CHECK_ERR,
	MEAL_ERR,
}	t_sem_error;

typedef struct s_monitor
{
	pid_t	pid;
	pid_t	*pid_array;
	int		philo_number;
	int		index;
	sem_t	*print_sem;
	sem_t	*check_sem;
	sem_t	*meal_sem;
}	t_monitor;

typedef struct s_philo
{
	int				full_tummy;
	int				monitor_ignore;
	long			start_time;
	int				death_time;
	int				number;
	int				time_to_sleep;
	int				time_to_eat;
	int				time_since_meal;
	int				meals_nb;
	int				meals_eaten;
	int				think_time;
	int				stop_program;
	sem_t			*forks;
	sem_t			*print_sem;
	sem_t			*check_sem;
	sem_t			*meal_sem;
}	t_philo;

typedef struct s_config
{
	pid_t			*pid_array;
	int				death;
	int				full_philos;
	int				meals_nb;
	int				philos_nb;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	long			start_time;
	pid_t			*philo_id;
	sem_t			*forks;
	sem_t			*print_sem;
	sem_t			*check_sem;
	sem_t			*meal_sem;
}	t_config;

void	print_log(char *str, int fd);
void	throw_args_error(void);
int		simple_atoi(const char *nptr);
int		check_number(int ac, char **av);
int		open_semaphores(t_config *conf);
void	close_semaphores(t_config *conf);

long	get_start_time(void);
long	get_current_time(long start_time);
void	ft_usleep(t_philo *philo, int time_ms, long start_time);

int		add_philo(t_config *conf, pid_t id[], int nb);
void	philo_routine(t_philo *philo);
void	monitor_philo(t_config *conf, pid_t philo_id[]);

void	try_to_write(t_philo *philo, char *message);
void	do_routine(t_philo *philo, sem_t *forks);

#endif
