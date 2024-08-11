/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_philos_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 14:03:19 by jteissie          #+#    #+#             */
/*   Updated: 2024/08/11 12:53:58 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static void	*wait_philos(void *arg)
{
	int			status;
	t_monitor	*monitor;

	monitor = (t_monitor *)arg;
	status = 0;
	waitpid(monitor->pid, &status, 0);
	if (WIFEXITED(status))
	{
		if (WEXITSTATUS(status) == EXIT_DEATH)
			sem_post(monitor->death_sem);
	}
	return (NULL);
}

void	monitor_philo(t_config *conf, pid_t philo_id[])
{
	t_monitor	monitor[MAX_PHILO];
	pthread_t	threads[MAX_PHILO];
	int			index;

	index = 0;
	memset(&monitor, 0, MAX_PHILO * sizeof(t_monitor));
	memset(&threads, 0, MAX_PHILO * sizeof(pthread_t));
	while (index < conf->philos_nb)
	{
		monitor[index].pid = philo_id[index];
		monitor[index].print_sem = conf->print_sem;
		monitor[index].death_sem = conf->death_sem;
		if (pthread_create(&threads[index], NULL, wait_philos, &monitor[index]) != 0)
			return ;
		index++;
	}
	index = 0;
	while (index < conf->philos_nb)
	{
		pthread_join(threads[index], NULL);
		index++;
	}
}
