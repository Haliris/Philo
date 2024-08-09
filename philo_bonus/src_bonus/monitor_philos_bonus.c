/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_philos_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jteissie <jteissie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 14:03:19 by jteissie          #+#    #+#             */
/*   Updated: 2024/08/09 11:30:48 by jteissie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static void	wait_philos(t_config *config, pthread_t philo_ids[])
{
	int	index;

	index = 0;
	while (index < config->philos_nb)
	{
		waitpid(philo_ids[index], NULL, 0);
		index++;
	}
}

static void	check_tummy(t_config *config, t_philo philos)
{
	if (config->meals_nb != -1)
	{
		if (philos.full_tummy == TRUE)
		{
			philos.full_tummy = FALSE;
			philos.monitor_ignore = TRUE;
			config->full_philos++;
		}
	}
}

static void	check_philo(t_philo philos, t_config *config, int *stop_run)
{
	int	curr_time;
	int	time_die;
	int	time_meal;

	time_die = config->time_to_die;
	time_meal = philos.time_since_meal;
	curr_time = get_current_time(config->start_time);
	check_tummy(config, philos);
	if (config->full_philos == config->philos_nb)
		*stop_run = TRUE;
	else if (curr_time - time_meal > time_die && philos.monitor_ignore == FALSE)
	{
		config->death = TRUE;
		//sem
		printf("%d %d has died.\n", curr_time, philos.number);
		//sem
		*stop_run = TRUE;
	}
}

void	monitor_philo(t_philo *philo[], t_config *conf, pthread_t philo_id[])
{
	int	index;
	int	stop_run;

	index = 0;
	stop_run = FALSE;
	while (stop_run == FALSE)
	{
		//sem
		check_philo(*philo[index], conf, &stop_run);
		//sem
		if (index == conf->philos_nb - 1)
			index = 0;
		else
		{
			usleep(10);
			index++;
		}
	}
	wait_philos(conf, philo_id);
	//unlink/close sem
}
