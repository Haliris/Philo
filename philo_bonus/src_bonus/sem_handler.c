/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sem_handler.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jteissie <jteissie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 13:43:42 by jteissie          #+#    #+#             */
/*   Updated: 2024/08/13 16:50:59 by jteissie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static void	panic_close_sem(t_config *conf, t_sem_error err)
{
	sem_close(conf->forks);
	sem_unlink("/forks");
	if (err == CHECK_ERR)
	{
		sem_close(conf->print_sem);
		sem_unlink("/print");
	}
	else if (err == MEAL_ERR)
	{
		sem_close(conf->print_sem);
		sem_unlink("/print");
		sem_close(conf->check_sem);
		sem_unlink("/check");
	}
	else if (err == DEATH_ERR)
	{
		sem_close(conf->print_sem);
		sem_unlink("/print");
		sem_close(conf->check_sem);
		sem_unlink("/check");
		sem_close(conf->meal_sem);
		sem_unlink("/meal");
	}
}

static void	unlink_prev_sems(void)
{
	sem_unlink("/forks");
	sem_unlink("/print");
	sem_unlink("/check");
	sem_unlink("/meal");
	sem_unlink("/death");
}

void	close_semaphores(t_config *conf)
{
	int	index;

	index = 0;
	while (index < conf->philos_nb)
	{
		sem_post(conf->meal_sem);
		sem_post(conf->check_sem);
		usleep(5000);
		index++;
	}
	sem_close(conf->forks);
	sem_unlink("/forks");
	sem_close(conf->print_sem);
	sem_unlink("/print");
	sem_close(conf->check_sem);
	sem_unlink("/check");
	sem_close(conf->meal_sem);
	sem_unlink("/meal");
	sem_close(conf->death_sem);
	sem_unlink("/death");
}

int	open_semaphores(t_config *conf)
{
	t_sem_error	status;

	status = OK;
	unlink_prev_sems();
	conf->forks = sem_open("/forks", O_CREAT, 0644, conf->philos_nb);
	if (conf->forks == SEM_FAILED)
		status = FORK_ERR;
	conf->print_sem = sem_open("/print", O_CREAT, 0644, 1);
	if (conf->print_sem == SEM_FAILED)
		status = PRINT_ERR;
	conf->check_sem = sem_open("/check", O_CREAT, 0644, 1);
	if (conf->check_sem == SEM_FAILED)
		status = CHECK_ERR;
	conf->meal_sem = sem_open("/meal", O_CREAT, 0644, 0);
	if (conf->meal_sem == SEM_FAILED)
		status = MEAL_ERR;
	conf->death_sem = sem_open("/death", O_CREAT, 0644, 1);
	if (conf->death_sem == SEM_FAILED)
		status = DEATH_ERR;
	if (status != OK)
	{
		panic_close_sem(conf, status);
		return (PANIC);
	}
	return (SUCCESS);
}
