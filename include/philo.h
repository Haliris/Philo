/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jteissie <jteissie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 13:52:12 by jteissie          #+#    #+#             */
/*   Updated: 2024/08/05 15:33:40 by jteissie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include "unistd.h"
# include "pthread.h"
# define FALSE 0
# define TRUE 1
# define SUCCESS 0
# define PARSE_ERRROR 1
# ifndef MAX_PHILO
#  define MAX_PHILO 200
# endif

typedef struct s_philo
{
	int	dead;
	int	time_to_sleep;
	int	time_to_eat;
	int	eating;
	int	meal_number;
	int	has_fork;
	int	think_time;
}	t_philo;

void	print_log(char *str, int fd);
void	throw_args_error(void);
int		simple_atoi(const char *nptr);
void	philo_bzero(void *s, size_t n);

void	routine(t_philo *config);
void	monitor_routine(int deat_time);
#endif
