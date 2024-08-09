/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jteissie <jteissie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 18:53:50 by jteissie          #+#    #+#             */
/*   Updated: 2024/08/09 15:27:40 by jteissie         ###   ########.fr       */
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
	if (philo->full_tummy == TRUE)
		return ;
	take_fork(philo, forks);
	try_to_write(philo, "is eating.");
	ft_usleep(philo, philo->time_to_eat, get_current_time(philo->start_time));
	sem_wait(philo->death_sem);
	philo->time_since_meal = get_current_time(philo->start_time);
	philo->meals_eaten++;
	sem_post(philo->death_sem);
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

void	philo_routine(t_philo *philo)
{
	sem_t	*forks;

	forks = philo->forks;
	sem_wait(philo->death_sem);
	philo->time_since_meal = get_current_time(philo->start_time);
	sem_post(philo->death_sem);
	if (philo->number % 2 == 0)
		usleep(philo->time_to_eat / 2);
	while (1)
	{
		check_stop(philo);
		try_to_eat(philo, forks);
		check_stop(philo);
		philo_sleep(philo);
		check_stop(philo);
		philo_think(philo);
	}
}
