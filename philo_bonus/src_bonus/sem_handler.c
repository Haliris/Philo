/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sem_handler.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 13:43:42 by jteissie          #+#    #+#             */
/*   Updated: 2024/08/11 17:30:02 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static void	panic_close_sem(t_config *conf, t_sem_error err)
{
	if (err == PRINT_ERR)
	{
		sem_close(conf->forks);
		sem_unlink("/forks");
	}
	else if (err == CHECK_ERR)
	{
		sem_close(conf->forks);
		sem_unlink("/forks");
		sem_close(conf->print_sem);
		sem_unlink("/print");
	}
}

static void	unlink_prev_sems(void)
{
	sem_unlink("/forks");
	sem_unlink("/print");
	sem_unlink("/check");
}

void	close_semaphores(t_config *conf)
{
	sem_close(conf->forks);
	sem_unlink("/forks");
	sem_close(conf->print_sem);
	sem_unlink("/print");
	sem_close(conf->check_sem);
	sem_unlink("/check");
}

int	open_semaphores(t_config *conf)
{
	t_sem_error	status;

	status = OK;
	unlink_prev_sems();
	conf->forks = sem_open("/forks", O_CREAT, 0644, conf->philos_nb);
	if (conf->forks == SEM_FAILED)
		status = FORK_ERR;
	if (status == OK)
	{
		conf->print_sem = sem_open("/print", O_CREAT, 0644, 1);
		if (conf->print_sem == SEM_FAILED)
			status = PRINT_ERR;
	}
	if (status == OK)
	{
		conf->check_sem = sem_open("/check", O_CREAT, 0644, 1);
		if (conf->check_sem == SEM_FAILED)
			status = CHECK_ERR;
	}
	if (status != OK)
	{
		panic_close_sem(conf, status);
		return (PANIC);
	}
	return (SUCCESS);
}
