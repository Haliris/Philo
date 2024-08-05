/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jteissie <jteissie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 17:51:14 by jteissie          #+#    #+#             */
/*   Updated: 2024/08/05 18:00:33 by jteissie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	destroy_mutexes(int philos_nb, pthread_mutex_t forks[])
{
	int	index;

	index = 0;
	while (index < philos_nb)
	{
		pthread_mutex_destroy(&forks[index]);
		index++;
	}
}

static void	panic_destroy_mutexes(pthread_mutex_t forks[], int index)
{
	while (index >= 0)
	{
		pthread_mutex_destroy(&forks[index]);
		index--;
	}
}

int	init_mutexes(int philos_nb, pthread_mutex_t forks[])
{
	int	index;

	index = 0;
	while (index < philos_nb)
	{
		if (pthread_mutex_init(&forks[index], NULL) != 0)
		{
			print_log("Mutex initialization failed!\n", STDERR_FILENO);
			panic_destroy_mutexes(forks, index);
			return (PANIC);
		}
		index++;
	}
	return (SUCCESS);
}
