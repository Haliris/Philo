/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jteissie <jteissie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 13:03:50 by jteissie          #+#    #+#             */
/*   Updated: 2024/08/09 15:45:49 by jteissie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	try_to_write(t_philo *philo, char *message)
{
	long	timestamp;

	timestamp = get_current_time(philo->start_time);
	sem_wait(philo->print_sem);
	printf("%ld %d %s\n", timestamp, philo->number, message);
	sem_post(philo->print_sem);
}

void	check_stop(t_philo *philo)
{
	sem_wait(philo->death_sem);
	if (philo->meals_eaten == philo->meals_nb)
	{
		philo->full_tummy = TRUE;
		sem_post(philo->death_sem);
		exit(EXIT_SUCCESS);
	}
	sem_post(philo->death_sem);
	return ;
}
