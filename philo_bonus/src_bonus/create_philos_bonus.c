/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_philos_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jteissie <jteissie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 14:13:23 by jteissie          #+#    #+#             */
/*   Updated: 2024/08/09 15:47:15 by jteissie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static void	kill_philos_panic(int nb, pid_t ids[])
{
	int	index;

	index = 0;

	while (index > nb)
	{
		kill(ids[index], SIGKILL);
		index++;
	}
}

static void	panic_free_philos(t_philo **philos, int n, pid_t id[], t_config *conf)
{
	int	index;

	index = 0;
	while (index < n)
	{
		if (n == conf->philos_nb)
			kill_philos_panic(conf->philos_nb, id);
		free(philos[index]);
		index++;
	}
	philos = NULL;
}

static int	make_fork(t_philo **philos, pid_t philo_ids[], t_config *conf)
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
		{
			kill_philos_panic(index, philo_ids);
			return (PANIC);
		}
		if (child_id == 0)
			philo_routine(philos[index]);
		else
		{
			philo_ids[index] = child_id;
			index++;
		}
	}
	return (SUCCESS);
}

static void	copy_conf(t_config *conf, t_philo *philos, int index)
{
	philos->forks = conf->forks;
	philos->number = index + 1;
	philos->print_sem = conf->print_sem;
	philos->time_to_eat = conf->time_to_eat;
	philos->death_time = conf->time_to_die;
	philos->time_to_sleep = conf->time_to_sleep;
	philos->death_sem = conf->death_sem;
	philos->meals_nb = conf->meals_nb;
	philos->meals_eaten = 0;
	philos->monitor_ignore = FALSE;
}

int	add_philo(t_config *conf, pid_t id[], int nb, t_philo **philo)
{
	int			counter;

	counter = 0;
	while (counter < nb)
	{
		philo[counter] = malloc(1 * sizeof(t_philo));
		if (!philo[counter])
		{
			panic_free_philos(philo, counter, id, conf);
			return (PANIC);
		}
		copy_conf(conf, philo[counter], counter);
		counter++;
	}
	if (make_fork(philo, id, conf) == PANIC)
	{
		panic_free_philos(philo, nb, id, conf);
		return (PANIC);
	}
	return (SUCCESS);
}
