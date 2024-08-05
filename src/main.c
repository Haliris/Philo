/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jteissie <jteissie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 13:45:17 by jteissie          #+#    #+#             */
/*   Updated: 2024/08/05 15:40:18 by jteissie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Do I need to protect pthread_create() ???

#include "philo.h"

void	create_philos(t_philo *config, pthread_t philo_list[])
{
	int			counter;
	pthread_t	philo_id;
	int			philos_nb;

	philos_nb = simple_atoi(av[1]);
	counter = 0;
	philo_id = 0;
	while (counter <= philos_nb)
	{
		pthread_create(&philo_id, NULL, routine, config);
		philo_list[counter] = philo_id;
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
	pthread_t	monitor_id;
	t_philo		philo_config;
	t_monitor	monitor_config;
	pthread_t	philo_array[MAX_PHILO];

	if (ac < 5 || ac > 6)
		throw_args_error();
	if (check_number(ac, av) == PARSE_ERROR)
		return (EXIT_FAILURE);
	philo_bzero(&philo_config, sizeof(philo_config));
	init_config(&philo_config);
	create_philos(&philo_config, &philo_array);
	pthread_join();
	return (EXIT_SUCCESS);
}
