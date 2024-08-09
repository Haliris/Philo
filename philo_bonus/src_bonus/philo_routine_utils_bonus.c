/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jteissie <jteissie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 13:03:50 by jteissie          #+#    #+#             */
/*   Updated: 2024/08/09 17:14:19 by jteissie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	try_to_write(t_philo *philo, char *message)
{
	long	timestamp;

	timestamp = -1;
	timestamp = get_current_time(philo->start_time);
	sem_wait(philo->print_sem);
	printf("%ld %d %s\n", timestamp, philo->number, message);
	sem_post(philo->print_sem);
}
