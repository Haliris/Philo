/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_philos.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jteissie <jteissie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 14:03:19 by jteissie          #+#    #+#             */
/*   Updated: 2024/08/12 13:29:29 by jteissie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	kill_philos(t_config *config, pthread_t philo_ids[])
{
	int	index;

	index = 0;
	while (index < config->philos_nb)
	{
		pthread_join(philo_ids[index], NULL);
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
		pthread_mutex_lock(&config->print_stick);
		printf("%d %d has died.\n", curr_time, philos.number);
		pthread_mutex_unlock(&config->print_stick);
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
		pthread_mutex_lock(&conf->death_lock);
		check_philo(*philo[index], conf, &stop_run);
		pthread_mutex_unlock(&conf->death_lock);
		if (index == conf->philos_nb - 1)
			index = 0;
		else
		{
			usleep(10);
			index++;
		}
	}
	kill_philos(conf, philo_id);
	destroy_mutexes(conf->philos_nb, conf);
}
