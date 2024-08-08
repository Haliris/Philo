/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jteissie <jteissie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 13:03:50 by jteissie          #+#    #+#             */
/*   Updated: 2024/08/08 16:57:16 by jteissie         ###   ########.fr       */
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
	if (philo->meals_eaten == philo->meals_nb)
	{
		philo->full_tummy = TRUE;
		pthread_mutex_unlock(philo->death_lock);
		return (TRUE);
	}
	pthread_mutex_unlock(philo->death_lock);
	return (FALSE);
}
