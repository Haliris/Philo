/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jteissie <jteissie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 13:45:17 by jteissie          #+#    #+#             */
/*   Updated: 2024/08/09 13:48:07 by jteissie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

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
	if (config->philos_nb > MAX_PHILO || config->philos_nb == 0)
	{
		print_log("Number of philos above MAX_PHILO number!\n", STDERR_FILENO);
		print_log("run 'Make MAX_PHILO=[number]'\n", STDERR_FILENO);
		return (PANIC);
	}
	return (SUCCESS);
}



int	main(int ac, char **av)
{
	t_config				conf;
	t_philo					*philo_structs[MAX_PHILO];
	sem_t					*forks;
	pid_t					philo_ids[MAX_PHILO];

	if (parse_args(&conf, ac, av) == PANIC)
		return (EXIT_FAILURE);
	init_config(&conf, av, ac);
	if (open_semaphores(&forks, &conf) == PANIC)
		return (EXIT_FAILURE);
	if (add_philo(&conf, philo_ids, conf.philos_nb, philo_structs) == PANIC)
	{
		return (EXIT_FAILURE);
	}
	monitor_philo(philo_structs, &conf, philo_ids);
	close_semaphores(&forks, &conf);
	free_philos(philo_structs, &conf);
	return (EXIT_SUCCESS);
}
