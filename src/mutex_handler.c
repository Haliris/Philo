/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jteissie <jteissie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 17:51:14 by jteissie          #+#    #+#             */
/*   Updated: 2024/08/08 14:35:56 by jteissie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#define OK 0
#define PRINT_STICK_ERR 1
#define DEATH_LOCK_ERR 2
#define ALL_ERR 3

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

static void	throw_mutex_error(t_config *config, int status)
{
	print_log("Mutex initialization failed!\n", STDERR_FILENO);
	if (status == PRINT_STICK_ERR)
		pthread_mutex_destroy(&config->print_stick);
	else if (status == DEATH_LOCK_ERR || status == ALL_ERR)
	{
		if (status == DEATH_LOCK_ERR)
			pthread_mutex_destroy(&config->print_stick);
		pthread_mutex_destroy(&config->death_lock);
	}
}

int	init_mutexes(int philos_nb, t_config *config)
{
	int	index;
	int	status;

	index = 0;
	status = OK;
	if (pthread_mutex_init(&config->print_stick, NULL) != 0)
		status = PRINT_STICK_ERR;
	if (pthread_mutex_init(&config->death_lock, NULL) != 0)
		status += DEATH_LOCK_ERR;
	while (index < philos_nb && status == OK)
	{
		if (pthread_mutex_init(&config->forks[index], NULL) != 0)
		{
			panic_destroy_mutexes(config->forks, index);
			throw_mutex_error(config, DEATH_LOCK_ERR);
			return (PANIC);
		}
		index++;
	}
	if (status != OK)
	{
		throw_mutex_error(config, status);
		return (PANIC);
	}
	return (SUCCESS);
}

#undef OK
#undef PRINT_STICK_ERR
#undef DEATH_LOCK_ERR
#undef ALL_ERR
