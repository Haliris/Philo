/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sem_handler.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jteissie <jteissie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 13:43:42 by jteissie          #+#    #+#             */
/*   Updated: 2024/08/09 13:48:09 by jteissie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static void	panic_close_sem(sem_t *forks, t_config *conf, t_sem_error err)
{
	if (err == PRINT_ERR)
	{
		sem_close(forks);
		sem_unlink("/forks");
	}
	else if (err == DEATH_ERR)
	{
		sem_close(forks);
		sem_unlink("/forks");
		sem_close(conf->print_sem);
		sem_unlink("/print");
	}
}

void	close_semaphore(sem_t *forks, t_config *conf)
{
	sem_close(forks);
	sem_unlink("/forks");
	sem_close(conf->death_sem);
	sem_unlink("/death");
	sem_close(conf->print_sem);
	sem_unlink("/print");
}

int	open_semaphores(sem_t *forks, t_config *conf)
{
	t_sem_error	status;

	status = OK;
	forks = sem_open("/forks", O_CREAT, 0644, conf->philos_nb);
	if (forks == SEM_FAILED)
		status = FORK_ERR;
	if (status == OK)
	{
		conf->print_sem = sem_open("/print", O_CREAT, 0644, 1);
		if (conf->print_sem == SEM_FAILED)
			status = PRINT_ERR;
	}
	if (status == OK)
	{
		conf->death_sem = sem_open("/print", O_CREAT, 0644, 1);
		if (conf->death_sem == SEM_FAILED)
			status = DEATH_ERR;
	}
	if (status != OK)
	{
		panic_close_sem(forks, conf, status);
		return (PANIC);
	}
	conf->forks = forks;
	return (SUCCESS);
}
