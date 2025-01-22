/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jteissie <jteissie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 18:53:50 by jteissie          #+#    #+#             */
/*   Updated: 2024/08/13 14:37:27 by jteissie         ###   ########.fr       */
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

void	exit_child(t_philo *philo, int code)
{
	philo->stop_program = TRUE;
	exit(code);
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
