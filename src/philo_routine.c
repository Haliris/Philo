/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jteissie <jteissie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 18:53:50 by jteissie          #+#    #+#             */
/*   Updated: 2024/08/06 19:37:44 by jteissie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	try_to_write(t_philo  *philo, char *message)
{
	pthread_mutex_lock(philo->print_stick);
	printf("%ld %d %s\n", get_current_time(philo->start_time), philo->number, message);
	pthread_mutex_unlock(philo->print_stick);
}

static void	try_to_eat(t_philo *philo, pthread_mutex_t *forks)
{
	pthread_mutex_lock(philo->meal_lock);
	if (philo->number % 2 == 0)
	{
		pthread_mutex_lock(&forks[philo->left_fork]);
		try_to_write(philo, "has taken a fork.");
		pthread_mutex_lock(&forks[philo->right_fork]);
		try_to_write(philo, "has taken a fork.");
	}
	else
	{
		pthread_mutex_lock(&forks[philo->right_fork]);
		try_to_write(philo, "has taken a fork.");
		pthread_mutex_lock(&forks[philo->left_fork]);
		try_to_write(philo, "has taken a fork.");

	}
	try_to_write(philo, "is eating.");
	ft_usleep(philo, philo->time_to_eat, get_current_time(philo->start_time));
	philo->time_since_meal = get_current_time(philo->start_time);
	philo->meals_nb++;
	pthread_mutex_unlock(&forks[philo->left_fork]);
	pthread_mutex_unlock(&forks[philo->right_fork]);
	pthread_mutex_unlock(philo->meal_lock);
}

static void	philo_sleep(t_philo *philo)
{
	try_to_write(philo, "is sleeping.");
	ft_usleep(philo, philo->time_to_sleep, get_current_time(philo->start_time));
}

void	*philo_routine(void *arg)
{
	t_philo			*philo;
	pthread_mutex_t	*forks;

	philo = (t_philo *)arg;
	forks = philo->forks;
	while (1)
	{
		pthread_mutex_lock(philo->death_lock);
		if (*philo->death == TRUE)
		{
			pthread_mutex_unlock(philo->death_lock);
			return (NULL);
		}
		pthread_mutex_unlock(philo->death_lock);
		try_to_eat(philo, forks);
		philo_sleep(philo);
		try_to_write(philo, "is thinking");
	}
}
