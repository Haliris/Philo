/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jteissie <jteissie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 17:51:14 by jteissie          #+#    #+#             */
/*   Updated: 2024/08/07 15:34:08 by jteissie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	destroy_mutexes(int philos_nb, t_config *config)
{
	int	index;

	index = 0;
	pthread_mutex_destroy(&config->print_stick);
	pthread_mutex_destroy(&config->death_lock);
	while (index < philos_nb)
	{
		if (config->forks_state[index] == LOCKED)
			pthread_mutex_unlock(&config->forks[index]);
		pthread_mutex_destroy(&config->forks[index]);
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

int	init_mutexes(int philos_nb, t_config *config)
{
	int	index;

	index = 0;
	if (pthread_mutex_init(&config->print_stick, NULL) != 0)
	{
			print_log("Mutex initialization failed!\n", STDERR_FILENO);
			pthread_mutex_destroy(&config->print_stick);
			return (PANIC);
	}
	if (pthread_mutex_init(&config->death_lock, NULL) != 0)
	{
			print_log("Mutex initialization failed!\n", STDERR_FILENO);
			pthread_mutex_destroy(&config->death_lock);
			return (PANIC);
	}
	while (index < philos_nb)
	{
		if (pthread_mutex_init(&config->forks[index], NULL) != 0)
		{
			print_log("Mutex initialization failed!\n", STDERR_FILENO);
			panic_destroy_mutexes(config->forks, index);
			return (PANIC);
		}
		index++;
	}
	return (SUCCESS);
}
