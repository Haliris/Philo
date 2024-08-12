/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_philos_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jteissie <jteissie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 14:03:19 by jteissie          #+#    #+#             */
/*   Updated: 2024/08/12 19:50:56 by jteissie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static void	kill_processes(t_monitor *monitor)
{
	int		index;
	int		philo_number;
	pid_t	*pids;

	index = 0;
	philo_number = monitor->philo_number;
	pids = monitor->pid_array;
	while (index < philo_number)
	{
		if (index == value)
		{
			index++;
			continue ;
		}
		kill(pids[index], SIGTERM); // do not kill the process that caused the kill_processes function
		index++;
	}
	index = 0;
	while (index < philo_number)
	{
		sem_post(monitor->stop_sem);
		index++;
	}
	index = 0;
	while (index < philo_number)
	{
		sem_post(monitor->check_sem);
		index++;
	}
}

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
			kill_processes(monitor);
	}
	return (NULL);
}

static void	monitor_cpy(t_monitor *m, t_config *conf, pid_t pid, pid_t *array)
{
	m->pid = pid;
	m->pid_array = array;
	m->print_sem = conf->print_sem;
	m->stop_sem = conf->stop_sem;
	m->check_sem = conf->check_sem;
	m->philo_number = conf->philos_nb;
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
		monitor_cpy(&monitor[index], conf, philo_id[index], philo_id);
		if (pthread_create(&threads[index], NULL,
				wait_philos, &monitor[index]) != 0)
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
