/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jteissie <jteissie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 13:45:17 by jteissie          #+#    #+#             */
/*   Updated: 2024/08/07 13:50:04 by jteissie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Do I need to protect pthread_create() ???

#include "philo.h"

int	create_philos(t_config *config, pthread_t philo_ids[], int philos_nb, t_philo **philos)
{
	int			counter;

	counter = 0;
	while (counter < philos_nb)
	{
		philos[counter] = malloc(sizeof(t_philo));
		if (!philos[counter])
			return (PANIC);
		if (counter == 0)
			philos[counter]->left_fork = philos_nb - 1;
		else
			philos[counter]->left_fork = counter - 1;
		philos[counter]->right_fork = counter;
		philos[counter]->number = counter + 1;
		philos[counter]->forks = config->forks;
		philos[counter]->meal_lock = config->meal_lock;
		philos[counter]->print_stick = config->print_stick;
		philos[counter]->time_to_eat = config->time_to_eat;
		philos[counter]->time_to_sleep = config->time_to_sleep;
		philos[counter]->start_time = config->start_time;
		philos[counter]->death = &config->death;
		philos[counter]->death_lock = config->death_lock;
		philos[counter]->forks_state = config->forks_state;
		philos[counter]->eating = &config->eat_flag;
		if (pthread_create(&philo_ids[counter], NULL, philo_routine, philos[counter]) != 0) //add safety here
			return (PANIC);
		counter++;
	}
	return (SUCCESS);
}

void	init_config(t_config *config, char ** av, int ac)
{
	config->time_to_die = simple_atoi(av[2]);
	config->time_to_eat = simple_atoi(av[3]);
	config->time_to_sleep = simple_atoi(av[4]);
	config->start_time = get_start_time();
	config->death = FALSE;
	if (ac == 6)
		config->meals_nb = simple_atoi(av[5]);
	else
		config->meals_nb = -1;
}

void	kill_philos(t_config *config, pthread_t philo_ids[])
{
	int	index;

	index = 0;
	while (index < config->philos_nb)
	{
		pthread_join(philo_ids[index], NULL);
		index++;
	}
}

void check_on_philo(t_philo philos, t_config *config, int *stop_run, pthread_t philo_ids[])
{
	int	current_time;

	current_time = get_current_time(config->start_time);
	if (current_time - philos.time_since_meal > config->time_to_die)
	{
		pthread_mutex_lock(config->death_lock);
		config->death = TRUE;
		pthread_mutex_lock(config->print_stick);
		printf("%d %d has died!", current_time, philos.number);
		*stop_run = TRUE;
		pthread_mutex_unlock(config->death_lock);
		kill_philos(config, philo_ids);
		pthread_mutex_unlock(config->print_stick);
		destroy_mutexes(config->philos_nb, config);
	}
	if (config->meals_nb != -1)
	{
		if (philos.meals_nb == config->meals_nb) //stop the philosophers
			*stop_run = TRUE;
	}
}

void monitor_philos(t_philo *philos[], t_config *config, pthread_t philo_ids[])
{
	int	index;
	int	stop_run;
	index = 0;

	stop_run = FALSE;
	while (stop_run == FALSE)
	{
		check_on_philo(*philos[index], config, &stop_run, philo_ids);
		if (index == config->philos_nb - 1)
			index = 0;
		else
		{
			usleep(10);
			index++;
		}
	}
}

int	main(int ac, char **av)
{
	t_config				config;
	t_philo					*philo_structs[MAX_PHILO];
	pthread_t				philo_ids[MAX_PHILO];
	int						index;

	index = 0;
	if (ac < 5 || ac > 6)
	{
		throw_args_error();
		return (EXIT_FAILURE);
	}
	memset(&config, 0, sizeof(t_config));
	if (check_number(ac, av) == PARSE_ERROR)
		return (EXIT_FAILURE);
	config.philos_nb = simple_atoi(av[1]);
	if (config.philos_nb > MAX_PHILO)
	{
		print_log("Number of philos above MAX_PHILO number!\n", STDERR_FILENO);
		print_log("run 'Make MAX_PHILO=[number]'\n", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	if (init_mutexes(config.philos_nb, &config) == PANIC)
		return (EXIT_FAILURE);
	init_config(&config, av, ac);
	if (create_philos(&config, philo_ids, config.philos_nb, philo_structs) == PANIC)
		return (EXIT_FAILURE); // ca leak
	monitor_philos(philo_structs, &config, philo_ids);
	//free philo array
	return (EXIT_SUCCESS);
}
