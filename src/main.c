/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jteissie <jteissie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 13:45:17 by jteissie          #+#    #+#             */
/*   Updated: 2024/08/05 17:58:49 by jteissie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Do I need to protect pthread_create() ???

#include "philo.h"

void	*test_routine(void *vargp)
{
	(void)vargp;
	printf("Hi, I am a thread\n");
	return NULL;
}

void	create_philos(t_philo *config, pthread_t philo_list[], int philos_nb)
{
	int			counter;

	(void)config;
	counter = 0;
	while (counter < philos_nb)
	{
		pthread_create(&philo_list[counter], NULL, test_routine, NULL);
		printf("thread: %lu\n", philo_list[counter]);
		counter++;
	}
}

void	init_config(t_philo *config, char ** av, int ac)
{
	config->dead = FALSE;
	config->has_fork = FALSE;
	config->think_time = 0;
	config->time_to_sleep = simple_atoi(av[3]);
	config->time_to_eat = simple_atoi(av[4]);
	if (ac == 6)
		config->meals_nb = simple_atoi(av[5]);
	else
		config->meals_nb = 0;
}

int	main(int ac, char **av)
{
	t_philo					philo_config;
	pthread_t				philo_array[MAX_PHILO];
	pthread_mutex_t			forks[MAX_PHILO];
	int						philos_nb;
	int						test_index;

	test_index = 0;
	if (ac < 5 || ac > 6)
	{
		throw_args_error();
		return (EXIT_FAILURE);
	}
	if (check_number(ac, av) == PARSE_ERROR)
		return (EXIT_FAILURE);
	memset(&philo_config, 0, sizeof(philo_config));
	init_config(&philo_config, av, ac);
	philos_nb = simple_atoi(av[1]);
	if (philos_nb > MAX_PHILO)
	{
		print_log("Number of philos above MAX_PHILO number!\n", STDERR_FILENO);
		print_log("run 'Make MAX_PHILO=[number]'\n", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	if (init_mutexes(philos_nb, forks) == PANIC)
		return (EXIT_FAILURE);
	create_philos(&philo_config, philo_array, philos_nb);
	while (test_index < philos_nb)
	{
		pthread_join(philo_array[test_index], NULL);
		test_index++;
	}
	destroy_mutexes(philos_nb, forks);
	return (EXIT_SUCCESS);
}
