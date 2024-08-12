/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_philos_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jteissie <jteissie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 14:13:23 by jteissie          #+#    #+#             */
/*   Updated: 2024/08/12 17:40:54 by jteissie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static void	kill_philos_panic(int nb, pid_t ids[])
{
	int	index;

	index = 0;
	while (index < nb)
	{
		kill(ids[index], SIGKILL);
		index++;
	}
}

static int	make_fork(t_philo *philos, pid_t philo_ids[], t_config *conf)
{
	int		index;
	pid_t	child_id;

	index = 0;
	conf->start_time = get_start_time();
	while (index < conf->philos_nb)
	{
		philos[index].start_time = conf->start_time;
		child_id = fork();
		if (child_id == -1)
		{
			kill_philos_panic(index, philo_ids);
			return (PANIC);
		}
		if (child_id == 0)
			philo_routine(&philos[index]);
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
	philos->full_tummy = FALSE;
	philos->number = index + 1;
	philos->print_sem = conf->print_sem;
	philos->forks = conf->forks;
	philos->check_sem = conf->check_sem;
	philos->time_to_eat = conf->time_to_eat;
	philos->death_time = conf->time_to_die;
	philos->time_to_sleep = conf->time_to_sleep;
	philos->meals_nb = conf->meals_nb;
	philos->meals_eaten = 0;
	philos->monitor_ignore = FALSE;
}

int	add_philo(t_config *conf, pid_t id[], int nb)
{
	int			counter;
	t_philo		philo[MAX_PHILO];

	counter = 0;
	memset(&philo, 0, MAX_PHILO * sizeof(t_philo));
	while (counter < nb)
	{
		copy_conf(conf, &philo[counter], counter);
		counter++;
	}
	if (make_fork(philo, id, conf) == PANIC)
	{
		kill_philos_panic(conf->philos_nb, id);
		return (PANIC);
	}
	return (SUCCESS);
}
