/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jteissie <jteissie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 17:51:14 by jteissie          #+#    #+#             */
/*   Updated: 2024/08/07 13:40:31 by jteissie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	destroy_mutexes(int philos_nb, t_config *config)
{
	int	index;

	index = 0;
	pthread_mutex_destroy(config->print_stick);
	pthread_mutex_destroy(config->death_lock);
	if (config->eat_flag == TRUE)
		pthread_mutex_unlock(config->meal_lock);
	pthread_mutex_destroy(config->meal_lock);
	free(config->print_stick);
	free(config->meal_lock);
	free(config->death_lock);
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

static int	allocate_memory(t_config *config)
{
	config->print_stick = malloc(1 * sizeof(pthread_mutex_t));
	if (!config->print_stick)
		return (PANIC);
	memset(config->print_stick, 0, sizeof(pthread_mutex_t));
	config->meal_lock = malloc(1 * sizeof(pthread_mutex_t));
	if (!config->meal_lock)
		return (PANIC);
	config->death_lock = malloc(1 * sizeof(pthread_mutex_t));
	if (!config->death_lock)
		return (PANIC);
	memset(config->meal_lock, 0, sizeof(pthread_mutex_t));
	return (SUCCESS);
}

int	init_mutexes(int philos_nb, t_config *config)
{
	int	index;

	index = 0;
	if (allocate_memory(config) == PANIC)
		return (PANIC);
	if (pthread_mutex_init(config->print_stick, NULL) != 0)
	{
			print_log("Mutex initialization failed!\n", STDERR_FILENO);
			pthread_mutex_destroy(config->print_stick);
			return (PANIC);
	}
	if (pthread_mutex_init(config->death_lock, NULL) != 0)
	{
			print_log("Mutex initialization failed!\n", STDERR_FILENO);
			pthread_mutex_destroy(config->death_lock);
			return (PANIC);
	}
	if (pthread_mutex_init(config->meal_lock, NULL) != 0)
	{
			print_log("Mutex initialization failed!\n", STDERR_FILENO);
			pthread_mutex_destroy(config->meal_lock);
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
