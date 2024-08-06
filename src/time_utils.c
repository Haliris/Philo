/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jteissie <jteissie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 18:55:19 by jteissie          #+#    #+#             */
/*   Updated: 2024/08/06 19:33:49 by jteissie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	get_start_time()
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
	{
		write(STDERR_FILENO, "gettimeofday() error.\n", 22);
		return (-1);
	}
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

long	get_current_time(long start_time)
{
	long	current_time;

	current_time = get_start_time();
	if (current_time == -1)
		return (-1);
	return (current_time - start_time);
}

void	ft_usleep(t_philo *philo, int time_ms, long start_time)
{
	long	sleep_time;

	sleep_time = time_ms;
	while (get_current_time(philo->start_time) - start_time < sleep_time)
		usleep(500);
}
