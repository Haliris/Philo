/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jteissie <jteissie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 13:03:50 by jteissie          #+#    #+#             */
/*   Updated: 2024/08/12 15:54:42 by jteissie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	try_to_write(t_philo *philo, char *message)
{
	long	timestamp;

	timestamp = get_current_time(philo->start_time);
	pthread_mutex_lock(philo->death_lock);
	if (*philo->death == TRUE)
	{
		pthread_mutex_unlock(philo->death_lock);
		return ;
	}
	pthread_mutex_unlock(philo->death_lock);
	pthread_mutex_lock(philo->print_stick);
	printf("%ld %d %s\n", timestamp, philo->number, message);
	pthread_mutex_unlock(philo->print_stick);
}

int	check_stop(t_philo *philo)
{
	pthread_mutex_lock(philo->death_lock);
	if (*philo->death == TRUE)
	{
		pthread_mutex_unlock(philo->death_lock);
		return (TRUE);
	}
	if (philo->meals_eaten == philo->meals_nb && philo->meal_ignore == FALSE)
	{
		philo->full_tummy = TRUE;
		pthread_mutex_unlock(philo->death_lock);
		return (FALSE);
	}
	pthread_mutex_unlock(philo->death_lock);
	return (FALSE);
}

static void	eat_alone(t_philo *philo, pthread_mutex_t *forks)
{
	long	timestamp;

	pthread_mutex_lock(&forks[philo->right_fork]);
	try_to_write(philo, "has taken a fork.");
	timestamp = get_current_time(philo->start_time);
	ft_usleep(philo, philo->death_time + 10, timestamp);
	pthread_mutex_unlock(&forks[philo->right_fork]);
}

void	*solo_routine(void *arg)
{
	t_philo			*philo;
	pthread_mutex_t	*forks;

	philo = (t_philo *)arg;
	forks = philo->forks;
	pthread_mutex_lock(philo->death_lock);
	philo->time_since_meal = get_current_time(philo->start_time);
	pthread_mutex_unlock(philo->death_lock);
	while (1)
	{
		if (check_stop(philo) == TRUE)
			return (NULL);
		eat_alone(philo, forks);
		if (check_stop(philo) == TRUE)
			return (NULL);
	}
}
