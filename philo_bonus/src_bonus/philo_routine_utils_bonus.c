/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jteissie <jteissie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 13:03:50 by jteissie          #+#    #+#             */
/*   Updated: 2024/08/08 19:07:01 by jteissie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	try_to_write(t_philo *philo, char *message)
{
	long	timestamp;

	timestamp = get_current_time(philo->start_time);
	//sem_wait/post_lock(philo->death_lock);
	if (*philo->death == TRUE)
	{
		//sem_wait/post_unlock(philo->death_lock);
		return ;
	}
	//sem_wait/post_unlock(philo->death_lock);
	//sem_wait/post_lock(philo->print_stick);
	printf("%ld %d %s\n", timestamp, philo->number, message);
	//sem_wait/post_unlock(philo->print_stick);
}

int	check_stop(t_philo *philo)
{
	//sem_wait/post_lock(philo->death_lock);
	if (*philo->death == TRUE)
	{
		//sem_wait/post_unlock(philo->death_lock);
		return (TRUE);
	}
	if (philo->meals_eaten == philo->meals_nb)
	{
		philo->full_tummy = TRUE;
		//sem_wait/post_unlock(philo->death_lock);
		return (TRUE);
	}
	//sem_wait/post_unlock(philo->death_lock);
	return (FALSE);
}
