/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_philos_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jteissie <jteissie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 14:03:19 by jteissie          #+#    #+#             */
/*   Updated: 2024/08/09 16:57:27 by jteissie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static void	wait_philos(t_config *config, pid_t philo_ids[])
{
	int	index;

	index = 0;
	while (index < config->philos_nb)
	{
		waitpid(philo_ids[index], NULL, 0);
		index++;
	}
}

static void	kill_philos(t_config *config, pid_t philo_ids[])
{
	int	index;

	index = 0;
	while (index < config->philos_nb)
	{
		kill(philo_ids[index], SIGKILL);
		index++;
	}
}

void	*check_death(void *arg)
{
	t_config	*conf;

	conf = (t_config *)arg;
	sem_wait(conf->death_sem);
	conf->death = TRUE;
	return (NULL);
}

void	*check_meal(void *arg)
{
	t_config	*config;
	int			full_nb;

	config = (t_config *)arg;
	full_nb = 0;
	while (full_nb < config->philos_nb)
	{
		sem_wait(config->meal_sem);
		full_nb++;
		sem_wait(config->meal_sem);
	}
	config->full_philos = TRUE;
	return (NULL);
}

void	monitor_philo(t_config *conf, pid_t philo_id[]) // REFACTOR MEAL CHECKING LOGIC
{
	pthread_t	death_monitor;
	pthread_t	meal_monitor;

	death_monitor = 0;
	meal_monitor = 0;
	sem_wait(conf->death_sem);
	sem_wait(conf->meal_sem);
	if (pthread_create(&death_monitor, NULL, check_death, conf) != 0)
	{
		//WTFFFFF
		return ;
	}
	if (pthread_create(&meal_monitor, NULL, check_meal, conf) != 0)
	{
		//WTFFFFF
		return ;
	}
	while (conf->full_philos == FALSE)
	{
		if (conf->death == TRUE)
		{
			kill_philos(conf, philo_id);
			return ;
		}
		else
			usleep(10);
	}
	wait_philos(conf, philo_id); //useless??
}
