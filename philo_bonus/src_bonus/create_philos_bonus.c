/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_philos_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jteissie <jteissie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 14:13:23 by jteissie          #+#    #+#             */
/*   Updated: 2024/08/08 19:12:49 by jteissie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static void	panic_free_philos(t_philo **philos, int size)
{
	int	index;

	index = 0;
	while (index < size)
	{
		free(philos[index]);
		index++;
	}
	philos = NULL;
}

static int	make_thread(t_philo **philos, pthread_t philo_ids[], t_config *conf)
{
	int		index;
	pid_t	child_id;

	index = 0;
	conf->start_time = get_start_time();
	while (index < conf->philos_nb)
	{
		philos[index]->start_time = conf->start_time;
		child_id = fork();
		if (child_id == -1)
			return (PANIC);
		if (child_id == 0)
			philo_routine(philos[index]);
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
			panic_free_philos(philo, counter);
			return (PANIC);
		}
		copy_conf(conf, philo[counter], nb, counter);
		counter++;
	}
	if (make_fork(philo, id, conf) == PANIC)
	{
		panic_free_philos(philo, nb);
		return (PANIC);
	}
	return (SUCCESS);
}
