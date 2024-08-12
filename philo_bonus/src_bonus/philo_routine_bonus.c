/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 18:53:50 by jteissie          #+#    #+#             */
/*   Updated: 2024/08/13 00:21:38 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	try_to_write(t_philo *philo, char *message)
{
	long	timestamp;

	timestamp = -1;
	timestamp = get_current_time(philo->start_time);
	sem_wait(philo->print_sem);
	printf("%ld %d %s\n", timestamp, philo->number, message);
	sem_post(philo->print_sem);
}

static void	exit_child(t_philo *philo, int code)
{
	philo->stop_program = TRUE;
	usleep(2000);
	// sem_close(philo->print_sem);
	// sem_close(philo->check_sem);
	// sem_close(philo->forks);
	exit(code);
}

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

static void	*check_stop(void *arg)
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

static void	*check_tummy(void *arg)
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

void	philo_routine(t_philo *philo)
{
	sem_t		*forks;
	pthread_t	meal_check;
	pthread_t	monitor;

	forks = philo->forks;
	monitor = 0;
	meal_check = 0;
	philo->time_since_meal = get_current_time(philo->start_time);
	if (pthread_create(&monitor, NULL, check_stop, philo) != 0)
		exit(EXIT_FAILURE);
	if (pthread_create(&meal_check, NULL, check_tummy, philo) != 0)
		exit(EXIT_FAILURE);
	pthread_detach(monitor);
	pthread_detach(meal_check);
	if (philo->number % 2 == 0)
		usleep((philo->time_to_eat / 2) * 1000);
	do_routine(philo, forks);
	exit_child(philo, EXIT_SUCCESS);
}
