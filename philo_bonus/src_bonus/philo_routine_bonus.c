/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 18:53:50 by jteissie          #+#    #+#             */
/*   Updated: 2024/08/11 17:03:47 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	try_to_write(t_philo *philo, char *message)
{
	long	timestamp;

	timestamp = -1;
	timestamp = get_current_time(philo->start_time);
	if (philo->kill_switch == TRUE || philo->dead == TRUE)
		return ;
	sem_wait(philo->print_sem);
	printf("%ld %d %s\n", timestamp, philo->number, message);
	sem_post(philo->print_sem);
}

static void	exit_child(t_philo *philo, pthread_t monitor, pthread_t kill_switch)
{
	pthread_join(monitor, NULL);
	if (philo->dead == TRUE)
		pthread_detach(kill_switch);
	else if (philo->full_tummy != TRUE)
		pthread_join(kill_switch, NULL);
	else if (philo->full_tummy == TRUE)
		pthread_detach(kill_switch);
	sem_close(philo->print_sem);
	sem_close(philo->death_sem);
	sem_close(philo->check_sem);
	sem_close(philo->forks);
	if (philo->dead == TRUE)
		exit(EXIT_DEATH);
	else
		exit(EXIT_SUCCESS);
}

static void	*check_stop(void *arg)
{
	t_philo	*philo;
	int		curr_time;
	int		time_meal;
	int		time_die;

	philo = (t_philo *)arg;
	time_die = philo->death_time;
	sem_wait(philo->check_sem);
	while (philo->kill_switch == FALSE && philo->full_tummy == FALSE)
	{
		curr_time = get_current_time(philo->start_time);
		time_meal = philo->time_since_meal;
		if (curr_time - time_meal > time_die)
		{
			sem_wait(philo->print_sem);
			printf("%d %d died.\n", curr_time, philo->number);
			sem_post(philo->print_sem);
			philo->dead = TRUE;
			sem_post(philo->check_sem);
			return (NULL);
		}
		sem_post(philo->check_sem);
		usleep(1000);
	}
	return (NULL);
}

static void	*kill_philo(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	sem_wait(philo->death_sem);
	sem_wait(philo->check_sem);
	philo->kill_switch = TRUE;
	sem_post(philo->check_sem);
	return (NULL);
}

void	philo_routine(t_philo *philo)
{
	sem_t		*forks;
	pthread_t	monitor;
	pthread_t	kill_switch;

	forks = philo->forks;
	philo->time_since_meal = get_current_time(philo->start_time);
	monitor = 0;
	if (pthread_create(&monitor, NULL, check_stop, philo) != 0)
		exit(EXIT_FAILURE);
	if (pthread_create(&kill_switch, NULL, kill_philo, philo) != 0)
		exit(EXIT_FAILURE);
	if (philo->number % 2 == 0)
		usleep((philo->time_to_eat / 2) * 1000);
	do_routine(philo, forks);
	exit_child(philo, monitor, kill_switch);
}
