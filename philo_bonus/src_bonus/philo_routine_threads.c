/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine_threads.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jteissie <jteissie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 14:35:42 by jteissie          #+#    #+#             */
/*   Updated: 2024/08/13 16:37:29 by jteissie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static void	check_death(t_philo *philo)
{
	int		curr_time;
	int		time_meal;
	int		time_die;

	time_die = philo->death_time;
	curr_time = get_current_time(philo->start_time);
	time_meal = philo->time_since_meal;
	if (curr_time - time_meal > time_die)
	{
		sem_wait(philo->print_sem);
		printf("%d %d died.\n", curr_time, philo->number);
		sem_post(philo->print_sem);
		sem_post(philo->check_sem);
		exit_child(philo, EXIT_DEATH);
	}
}

void	*check_stop(void *arg)
{
	t_philo		*philo;
	int			stop;

	philo = (t_philo *)arg;
	stop = philo->stop_program;
	while (stop == FALSE)
	{
		if (stop == TRUE)
			return (NULL);
		sem_wait(philo->check_sem);
		if (stop == TRUE)
			return (NULL);
		check_death(philo);
		if (philo->full_tummy == TRUE)
		{
			sem_post(philo->check_sem);
			return (NULL);
		}
		sem_post(philo->check_sem);
		if (stop == TRUE)
			return (NULL);
		usleep(500);
	}
	return (NULL);
}

void	*check_tummy(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->meals_nb == -1)
		return (NULL);
	while (TRUE)
	{
		sem_wait(philo->check_sem);
		if (philo->meals_eaten == philo->meals_nb)
		{
			sem_post(philo->meal_sem);
			sem_post(philo->check_sem);
			return (NULL);
		}
		sem_post(philo->check_sem);
		usleep(1000);
	}
}
