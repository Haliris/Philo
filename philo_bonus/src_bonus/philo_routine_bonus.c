/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jteissie <jteissie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 18:53:50 by jteissie          #+#    #+#             */
/*   Updated: 2024/08/09 17:22:11 by jteissie         ###   ########.fr       */
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
	philo->time_since_meal = get_current_time(philo->start_time);
	philo->meals_eaten++;
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

void	*check_stop(void *arg)
{
	t_philo	*philo;
	int		curr_time;
	int		time_meal;
	int		time_die;

	philo = (t_philo *)arg;
	time_die = philo->death_time;
	while (1)
	{
		curr_time = get_current_time(philo->start_time);
		time_meal = philo->time_since_meal;
		if (curr_time - time_meal > time_die)
		{
			sem_wait(philo->print_sem);
			printf("%d %d has died.\n", curr_time, philo->number);
			sem_post(philo->print_sem);
			sem_post(philo->death_sem);
			philo->dead = TRUE;
			return (NULL);
		}
		usleep(10);
	}
}

void	philo_routine(t_philo *philo)
{
	sem_t		*forks;
	pthread_t	monitor;

	forks = philo->forks;
	philo->time_since_meal = get_current_time(philo->start_time);
	monitor = 0;
	if (pthread_create(&monitor, NULL, check_stop, philo) != 0)
		exit(EXIT_FAILURE);
	if (philo->number % 2 == 0)
		usleep(philo->time_to_eat / 2);
	while (1)
	{
		if (philo->dead == TRUE)
			exit(EXIT_SUCCESS);
		try_to_eat(philo, forks);
		if (philo->dead == TRUE)
			exit(EXIT_SUCCESS);
		if (philo->meals_eaten == philo->meals_nb)
		{
			sem_post(philo->meal_sem);
			exit(EXIT_SUCCESS);
		}
		philo_sleep(philo);
		if (philo->dead == TRUE)
			exit(EXIT_SUCCESS);
		philo_think(philo);
		if (philo->dead == TRUE)
			exit(EXIT_SUCCESS);
	}
}
