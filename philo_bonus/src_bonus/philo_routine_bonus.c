/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jteissie <jteissie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 18:53:50 by jteissie          #+#    #+#             */
/*   Updated: 2024/08/12 19:55:08 by jteissie         ###   ########.fr       */
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
	if (code == EXIT_DEATH)
	{
		//"free" the monitoring thread
	}
	sem_close(philo->print_sem);
	sem_close(philo->check_sem);
	sem_close(philo->forks);
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

void	*stop_program(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	sem_wait(philo->stop_sem);
	printf("stop_program set to true\n");
	philo->stop_program = TRUE;
	return (NULL);
}

static void	*check_stop(void *arg)
{
	t_philo		*philo;
	pthread_t	stop_monitor;
	int			stop;

	philo = (t_philo *)arg;
	stop = philo->stop_program;
	if (pthread_create(&stop_monitor, NULL, stop_program, philo) != 0)
		return (NULL);
	pthread_detach(stop_monitor);
	while (stop == FALSE)
	{
		if (stop == TRUE)
			exit(EXIT_SUCCESS);
		sem_wait(philo->check_sem);
		if (stop == TRUE)
			exit(EXIT_SUCCESS);
		check_death(philo);
		if (philo->full_tummy == TRUE)
		{
			sem_post(philo->check_sem);
			return (NULL);
		}
		sem_post(philo->check_sem);
		if (stop == TRUE)
			exit(EXIT_SUCCESS);
		usleep(1000);
	}
	return (NULL);
}

void	philo_routine(t_philo *philo)
{
	sem_t		*forks;
	pthread_t	monitor;

	forks = philo->forks;
	philo->time_since_meal = get_current_time(philo->start_time);
	monitor = 0;
	check_sem_name = ft_itoa(philo->number); //check_sem naming logic
	if (pthread_create(&monitor, NULL, check_stop, philo) != 0)
		exit(EXIT_FAILURE);
	pthread_detach(monitor);
	if (philo->number % 2 == 0)
		usleep((philo->time_to_eat / 2) * 1000);
	do_routine(philo, forks);
	exit_child(philo, EXIT_SUCCESS);
}
