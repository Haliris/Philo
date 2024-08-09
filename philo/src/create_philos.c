/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_philos.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jteissie <jteissie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 14:13:23 by jteissie          #+#    #+#             */
/*   Updated: 2024/08/09 11:43:34 by jteissie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	panic_free_philos(t_config *conf, t_philo **philos, int size, pthread_t ids[])
{
	int	index;

	index = 0;
	pthread_mutex_lock(&conf->death_lock);
	print_log("CATASTROPHIC CREATE_PHILOS EXCEPTION.", STDERR_FILENO);
	conf->death = TRUE;
	pthread_mutex_unlock(&conf->death_lock);
	while (index < size)
	{
		if (size == conf->philos_nb)
			pthread_join(ids[index], NULL);
		free(philos[index]);
		index++;
	}
	philos = NULL;
}

static int	make_single_philo(t_philo **philo, pthread_t id[], t_config *conf)
{
	conf->start_time = get_start_time();
	philo[0]->start_time = conf->start_time;
	if (pthread_create(&id[0], NULL, solo_routine, philo[0]) != 0)
	{
		free(philo[0]);
		return (PANIC);
	}
	return (SUCCESS);
}

static int	make_thread(t_philo **philos, pthread_t philo_ids[], t_config *conf)
{
	int	index;

	index = 0;
	conf->start_time = get_start_time();
	while (index < conf->philos_nb)
	{
		philos[index]->start_time = conf->start_time;
		if (pthread_create(&philo_ids[index],
				NULL, philo_routine, philos[index]) != 0)
			return (PANIC);
		index++;
	}
	return (SUCCESS);
}

static void	copy_conf(t_config *conf, t_philo *philos, int philos_nb, int count)
{
	if (count == 0)
		philos->left_fork = philos_nb - 1;
	else
		philos->left_fork = count - 1;
	philos->right_fork = count;
	philos->number = count + 1;
	philos->forks = conf->forks;
	philos->print_stick = &conf->print_stick;
	philos->time_to_eat = conf->time_to_eat;
	philos->death_time = conf->time_to_die;
	philos->time_to_sleep = conf->time_to_sleep;
	philos->death = &conf->death;
	philos->death_lock = &conf->death_lock;
	philos->forks_state = conf->forks_state;
	philos->meals_nb = conf->meals_nb;
	philos->meals_eaten = 0;
	philos->monitor_ignore = FALSE;
}

int	add_philo(t_config *conf, pthread_t id[], int nb, t_philo **philo)
{
	int			counter;

	counter = 0;
	while (counter < nb)
	{
		philo[counter] = malloc(1 * sizeof(t_philo));
		if (!philo[counter])
		{
			panic_free_philos(conf, philo, counter, id);
			return (PANIC);
		}
		copy_conf(conf, philo[counter], nb, counter);
		counter++;
	}
	if (nb == 1)
	{
		if (make_single_philo(philo, id, conf) == PANIC)
			return (PANIC);
	}
	else if (make_thread(philo, id, conf) == PANIC)
	{
		panic_free_philos(conf, philo, counter, id);
		return (PANIC);
	}
	return (SUCCESS);
}
