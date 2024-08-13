/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jteissie <jteissie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 13:52:48 by jteissie          #+#    #+#             */
/*   Updated: 2024/08/13 15:01:56 by jteissie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	print_log(char *str, int fd)
{
	int	i;

	i = 0;
	if (!str)
		return ;
	while (str[i])
	{
		write(fd, &str[i], 1);
		i++;
	}
}

int	check_number(int ac, char **av)
{
	int		av_i;
	int		str_i;
	char	*str;

	str_i = 0;
	av_i = 1;
	while (av_i < ac)
	{
		str = av[av_i];
		while (str[str_i])
		{
			if (str[str_i] < '0' || str[str_i] > '9')
			{
				print_log("Error parsing args ", STDERR_FILENO);
				print_log(str, STDERR_FILENO);
				print_log(" is not a positive number\n", STDERR_FILENO);
				return (PARSE_ERROR);
			}
			str_i++;
		}
		str_i = 0;
		av_i++;
	}
	return (SUCCESS);
}

int	simple_atoi(const char *nptr)
{
	int	i;
	int	result;
	int	minus;

	i = 0;
	result = 0;
	minus = 0;
	while (nptr[i])
	{
		result = result * 10 + (nptr[i] - '0');
		i++;
	}
	if (minus)
		result *= -1;
	return (result);
}

void	throw_args_error(void)
{
	print_log("Error, expected [number_of_philosophers]", STDERR_FILENO);
	print_log(" [time_to_die]", STDERR_FILENO);
	print_log(" [time_to_eat]", STDERR_FILENO);
	print_log(" [time_to_sleep]", STDERR_FILENO);
	print_log(" [number_of_times_to_eat] (optional)\n", STDERR_FILENO);
}
