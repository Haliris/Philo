/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine_handler.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 13:03:50 by jteissie          #+#    #+#             */
/*   Updated: 2024/08/11 18:39:55 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static void	take_fork(t_philo *philo, sem_t *forks)
{
	if (philo->number % 2 == 0)
	{
		sem_wait(forks);
		try_to_write(philo, "has taken a fork.");
		sem_wait(forks);
		try_to_write(philo, "has taken a fork.");
	}
	else if (philo->number % 2 != 0)
	{
		sem_wait(forks);
		try_to_write(philo, "has taken a fork.");
		sem_wait(forks);
		try_to_write(philo, "has taken a fork.");
	}
}

static void	try_to_eat(t_philo *philo, sem_t *forks)
{
	take_fork(philo, forks);
	try_to_write(philo, "is eating.");
	ft_usleep(philo, philo->time_to_eat, get_current_time(philo->start_time));
	sem_wait(philo->check_sem);
	philo->time_since_meal = get_current_time(philo->start_time);
	philo->meals_eaten++;
	sem_post(philo->check_sem);
	sem_post(forks);
	sem_post(forks);
}

static void	philo_sleep(t_philo *philo)
{
	try_to_write(philo, "is sleeping.");
	ft_usleep(philo, philo->time_to_sleep, get_current_time(philo->start_time));
}

static void	philo_think(t_philo *philo)
{
	long	time_left;
	long	timestamp;
	long	reserved_time;

	timestamp = get_current_time(philo->start_time);
	reserved_time = timestamp - philo->time_since_meal;
	time_left = philo->death_time - reserved_time;
	try_to_write(philo, "is thinking");
	if (time_left >= philo->time_to_eat)
		ft_usleep(philo, (time_left - philo->time_to_eat) * 0.90, timestamp);
}

void	do_routine(t_philo *philo, sem_t *forks)
{
	while (TRUE)
	{
		try_to_eat(philo, forks);
		philo_sleep(philo);
		philo_think(philo);
		if (philo->meals_eaten == philo->meals_nb)
		{
			sem_wait(philo->check_sem);
			philo->full_tummy = TRUE;
			sem_post(philo->check_sem);
			break ;
		}
	}
	usleep(1000);
}