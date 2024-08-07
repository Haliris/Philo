/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jteissie <jteissie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 18:53:50 by jteissie          #+#    #+#             */
/*   Updated: 2024/08/07 17:01:19 by jteissie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	try_to_write(t_philo  *philo, char *message)
{
	pthread_mutex_lock(philo->death_lock);
	if (*philo->death == TRUE)
	{
		pthread_mutex_unlock(philo->death_lock);
		return ;
	}
	pthread_mutex_unlock(philo->death_lock);
	pthread_mutex_lock(philo->print_stick);
	printf("%ld %d %s\n", get_current_time(philo->start_time), philo->number, message);
	pthread_mutex_unlock(philo->print_stick);
}

static void	try_to_eat(t_philo *philo, pthread_mutex_t *forks)
{
	if (philo->full_tummy == TRUE)
		return ;
	if (philo->number % 2 == 0)
	{
		pthread_mutex_lock(&forks[philo->left_fork]);
		philo->forks_state[philo->left_fork] = LOCKED;
		try_to_write(philo, "has taken a fork.");
		pthread_mutex_lock(&forks[philo->right_fork]);
		philo->forks_state[philo->right_fork] = LOCKED;
		try_to_write(philo, "has taken a fork.");
	}
	else if (philo->number % 2 != 0)
	{
		pthread_mutex_lock(&forks[philo->right_fork]);
		philo->forks_state[philo->right_fork] = LOCKED;
		try_to_write(philo, "has taken a fork.");
		pthread_mutex_lock(&forks[philo->left_fork]);
		philo->forks_state[philo->left_fork] = LOCKED;
		try_to_write(philo, "has taken a fork.");

	}
	try_to_write(philo, "is eating.");
	ft_usleep(philo, philo->time_to_eat, get_current_time(philo->start_time));
	philo->time_since_meal = get_current_time(philo->start_time);
	if (philo->meals_nb != -1)
		philo->meals_eaten++;
	philo->forks_state[philo->left_fork] = UNLOCKED;
	philo->forks_state[philo->right_fork] = UNLOCKED;
	pthread_mutex_unlock(&forks[philo->left_fork]);
	pthread_mutex_unlock(&forks[philo->right_fork]);
}

static void	philo_sleep(t_philo *philo)
{
	try_to_write(philo, "is sleeping.");
	ft_usleep(philo, philo->time_to_sleep, get_current_time(philo->start_time));
}

static int	check_stop(t_philo *philo)
{
	pthread_mutex_lock(philo->death_lock);
	if (*philo->death == TRUE)
	{
		pthread_mutex_unlock(philo->death_lock);
		return (TRUE);
	}
	pthread_mutex_unlock(philo->death_lock);
	if (philo->meals_eaten == philo->meals_nb)
		philo->full_tummy = TRUE;
	return (FALSE);
}

void	print_variable(t_philo *philo, int time)
{
	pthread_mutex_lock(philo->print_stick);
	printf("time: %d\n", time);
	pthread_mutex_unlock(philo->print_stick);
}

static void	philo_think(t_philo *philo)
{
	int	time_left;

	time_left = philo->death_time - (get_current_time(philo->start_time) - philo->time_since_meal) - 10;
	try_to_write(philo, "is thinking");
	ft_usleep(philo, (time_left - philo->time_to_eat) * 0.90, get_current_time(philo->start_time));
}

void	*philo_routine(void *arg)
{
	t_philo			*philo;
	pthread_mutex_t	*forks;

	philo = (t_philo *)arg;
	forks = philo->forks;
	philo->time_since_meal = get_current_time(philo->start_time);
	while (1)
	{
		if (philo->full_tummy == TRUE)
			continue ;
		if (check_stop(philo) == TRUE)
			return (NULL);
		try_to_eat(philo, forks);
		if (check_stop(philo) == TRUE)
			return (NULL);
		if (philo->full_tummy == TRUE)
			continue ;
		philo_sleep(philo);
		if (check_stop(philo) == TRUE)
			return (NULL);
		if (philo->full_tummy == TRUE)
			continue ;
		philo_think(philo);
	}
}
