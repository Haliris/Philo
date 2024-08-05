/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_current_time.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jteissie <jteissie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 18:55:19 by jteissie          #+#    #+#             */
/*   Updated: 2024/08/05 18:55:27 by jteissie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	get_current_time()
{
	struct timeval	time;

	if (gettimeofday(time, NULL) == -1)
		write(STDERR_FILENO, "gettimeofday() error.\n", 22);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

void	ft_usleep(int time_ms)
{
	int	start;

	start = get_current_time();
	while (get_current_time() - start < time_ms)
	{
		usleep(500);
	}
}
