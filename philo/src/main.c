/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jteissie <jteissie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 13:45:17 by jteissie          #+#    #+#             */
/*   Updated: 2024/08/13 14:51:04 by jteissie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	init_config(t_config *config, char **av, int ac)
{
	config->time_to_die = simple_atoi(av[2]);
	config->time_to_eat = simple_atoi(av[3]);
	config->time_to_sleep = simple_atoi(av[4]);
	config->full_philos = 0;
	config->death = FALSE;
	if (ac == 6)
		config->meals_nb = simple_atoi(av[5]);
	else
		config->meals_nb = -1;
}

static void	free_philos(t_philo **philo_structs, t_config *config)
{
	int	index;

	index = 0;
	while (index < config->philos_nb)
	{
		free(philo_structs[index]);
		index++;
	}
	philo_structs = NULL;
}

static int	parse_args(t_config *config, int ac, char **av)
{
	if (ac < 5 || ac > 6)
	{
		throw_args_error();
		return (PANIC);
	}
	memset(config, 0, sizeof(t_config));
	if (check_number(ac, av) == PARSE_ERROR)
		return (PANIC);
	config->philos_nb = simple_atoi(av[1]);
	if (config->philos_nb > MAX_PHILO || config->philos_nb <= 0)
	{
		print_log("Invalid number of philosophers.\n", STDERR_FILENO);
		print_log("run 'make MAX_PHILO=[number]'\n", STDERR_FILENO);
		return (PANIC);
	}
	return (SUCCESS);
}

int	main(int ac, char **av)
{
	t_config				conf;
	t_philo					*philo_structs[MAX_PHILO];
	pthread_t				philo_id[MAX_PHILO];

	if (parse_args(&conf, ac, av) == PANIC)
		return (EXIT_FAILURE);
	if (init_mutexes(conf.philos_nb, &conf) == PANIC)
		return (EXIT_FAILURE);
	init_config(&conf, av, ac);
	if (conf.meals_nb == 0)
		return (EXIT_SUCCESS);
	if (add_philo(&conf, philo_id, conf.philos_nb, philo_structs) == PANIC)
	{
		destroy_mutexes(conf.philos_nb, &conf);
		return (EXIT_FAILURE);
	}
	monitor_philo(philo_structs, &conf, philo_id);
	free_philos(philo_structs, &conf);
	return (EXIT_SUCCESS);
}
