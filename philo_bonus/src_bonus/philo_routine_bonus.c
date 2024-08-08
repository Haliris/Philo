/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jteissie <jteissie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 18:53:50 by jteissie          #+#    #+#             */
/*   Updated: 2024/08/08 19:13:20 by jteissie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static void	take_fork(t_philo *philo, //sem_wait/post_t *forks)
{
	if (philo->number % 2 == 0)
	{
		//sem_wait/post_lock(&forks[philo->left_fork]);
		try_to_write(philo, "has taken a fork.");
		//sem_wait/post_lock(&forks[philo->right_fork]);
		try_to_write(philo, "has taken a fork.");
	}
	else if (philo->number % 2 != 0)
	{
		//sem_wait/post_lock(&forks[philo->right_fork]);
		try_to_write(philo, "has taken a fork.");
		//sem_wait/post_lock(&forks[philo->left_fork]);
		try_to_write(philo, "has taken a fork.");
	}
}

static void	try_to_eat(t_philo *philo, //sem_wait/post_t *forks)
{
	if (philo->full_tummy == TRUE)
		return ;
	take_fork(philo, forks);
	try_to_write(philo, "is eating.");
	ft_usleep(philo, philo->time_to_eat, get_current_time(philo->start_time));
	//sem_wait/post_lock(philo->death_lock);
	philo->time_since_meal = get_current_time(philo->start_time);
	philo->meals_eaten++;
	//sem_wait/post_unlock(philo->death_lock);
	//sem_wait/post_unlock(&forks[philo->left_fork]);
	//sem_wait/post_unlock(&forks[philo->right_fork]);
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

void	*philo_routine(t_philo *philo)
{
	//sem_wait/post_t	*forks;

	//sem_wait/post_lock(philo->death_lock);
	philo->time_since_meal = get_current_time(philo->start_time);
	//sem_wait/post_unlock(philo->death_lock);
	if (philo->number % 2 == 0)
		usleep(500);
	while (1)
	{
		if (check_stop(philo) == TRUE)
			return (NULL);
		try_to_eat(philo, forks);
		if (check_stop(philo) == TRUE)
			return (NULL);
		philo_sleep(philo);
		if (check_stop(philo) == TRUE)
			return (NULL);
		philo_think(philo);
	}
}
