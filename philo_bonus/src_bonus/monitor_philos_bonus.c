/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_philos_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 14:03:19 by jteissie          #+#    #+#             */
/*   Updated: 2024/08/11 18:28:53 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static void	kill_processes(pid_t *pids, int philo_number)
{
	int	index;

	index = 0;
	while (index < philo_number)
	{
		kill(pids[index], SIGKILL);
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
			kill_processes(monitor->pid_array, monitor->philo_number);
	}
	return (NULL);
}

static void	copy_conf_monitor(t_monitor *monitor, t_config *conf, pid_t pid, pid_t *pid_array)
{
	monitor->pid = pid;
	monitor->pid_array = pid_array;
	monitor->print_sem = conf->print_sem;
	monitor->philo_number = conf->philos_nb;
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
		copy_conf_monitor(&monitor[index], conf, philo_id[index], philo_id);	
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
