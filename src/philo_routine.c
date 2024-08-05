/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jteissie <jteissie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 18:53:50 by jteissie          #+#    #+#             */
/*   Updated: 2024/08/05 18:54:15 by jteissie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	try_to_eat(t_philo *philo, pthread_mutex_t *forks)
{
	pthread_mutex_lock(&forks[philo->left_fork]);
	printf("%d %d has taken a fork\n", 0, 0);
	pthread_mutex_lock(&forks[philo->right_fork]);
	printf("%d %d has taken a fork\n", 0, 0);
	printf("%d %d is eating\n", 0, 0);
	ft_usleep(philo->time_to_eat);
	philo->time_since_meal = get_current_time();
	pthread_mutex_unlock(&forks[philo->left_fork]);
	pthread_mutex_unlock(&forks[philo->right_fork]);
}

static void	philo_sleep(t_philo *philo)
{
	printf("%d %d is sleeping\n", 0, 0);
	ft_usleep(philo->time_to_sleep);
}

void	*philo_routine(void *arg)
{
	t_philo			*philo;
	pthread_mutex_t	*forks;

	philo = *(*t_philo)arg;
	forks = philo->forks;
	while (1)
	{
		try_to_eat(philo, forks);
		philo_sleep(philo);
		printf("%d %d is thinking\n", 0, 0);
	}
}
