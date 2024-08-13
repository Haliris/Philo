/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_philos_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jteissie <jteissie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 14:03:19 by jteissie          #+#    #+#             */
/*   Updated: 2024/08/13 14:33:20 by jteissie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static void	kill_processes(int philo_number, pid_t *pids, int ignore)
{
	int		index;

	index = 0;
	while (index < philo_number)
	{
		if (index == ignore)
		{
			index++;
			continue ;
		}
		kill(pids[index], SIGTERM);
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
			kill_processes(monitor->philo_number,
				monitor->pid_array, monitor->index);
	}
	return (NULL);
}

static void	monitor_cpy(t_monitor *m, t_config *conf, pid_t pid, pid_t *array)
{
	m->pid = pid;
	m->pid_array = array;
	m->print_sem = conf->print_sem;
	m->meal_sem = conf->meal_sem;
	m->check_sem = conf->check_sem;
	m->philo_number = conf->philos_nb;
}

static void	*meal_monitor(void *arg)
{
	t_config	*config;
	int			full_philos;

	config = (t_config *)arg;
	full_philos = 0;
	if (config->meals_nb == -1)
		return (NULL);
	while (full_philos < config->philos_nb)
	{
		sem_wait(config->meal_sem);
		full_philos++;
	}
	kill_processes(config->philos_nb, config->pid_array, -1);
	return (NULL);
}

void	monitor_philo(t_config *conf, pid_t philo_id[])
{
	t_monitor	monitor[MAX_PHILO];
	pthread_t	threads[MAX_PHILO];
	pthread_t	meal_check;
	int			index;

	index = 0;
	memset(&monitor, 0, MAX_PHILO * sizeof(t_monitor));
	memset(&threads, 0, MAX_PHILO * sizeof(pthread_t));
	if (pthread_create(&meal_check, NULL, meal_monitor, conf) != 0)
		return ;
	pthread_detach(meal_check);
	while (index < conf->philos_nb)
	{
		monitor_cpy(&monitor[index], conf, philo_id[index], philo_id);
		monitor[index].index = index;
		if (pthread_create(&threads[index], NULL,
				wait_philos, &monitor[index]) != 0)
			return (kill_processes(conf->philos_nb, philo_id, -1));
		pthread_join(threads[index], NULL);
		index++;
	}
}
