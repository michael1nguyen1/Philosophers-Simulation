/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linhnguy <linhnguy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 21:47:32 by linhnguy          #+#    #+#             */
/*   Updated: 2024/07/05 14:50:05 by linhnguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	put_error_fd(int fd, char *str)
{
	int len;
	
	len = 0;
	while (str[len])
		len++;
	write(fd, str, len);
	return (-1);
}

void	print_actions(t_philo *data, char *str)
{
	pthread_mutex_lock(data->print);
	printf("%d %d %s\n", current_time(data), data->philo_id, str);
	pthread_mutex_unlock(data->print);
}

int	check_args(int argc, char **argv)
{
	int i;
	int j;

	i = 1;
	while (i < argc)
	{
		j = 0;
		while (argv[i][j])
		{
			if (argv[i][j] != '+' && (argv[i][j] < '0' || argv[i][j] > '9'))
				return (-1);
			j++;
		}
		i++;
	}
	return (0);
}

int	init_struct(int argc, char** argv, t_philo **data , pthread_mutex_t **forks)
{
	int i;

	i = 0;
	*data = malloc(sizeof(t_philo) * ft_atoi(argv[1]));
	if (!*data)
		return (put_error_fd(2, "malloc failed in main\n"));
	while (i < ft_atoi(argv[1]))
	{	
		(*data)[i].philo = ft_atoi(argv[1]);
		(*data)[i].die_time = ft_atoi(argv[2]);
		(*data)[i].eat_time = ft_atoi(argv[3]);
		(*data)[i].sleep_time = ft_atoi(argv[4]);
		if (argc == 6)
			(*data)[i].max_meals = ft_atoi(argv[5]);
		else
			(*data)[i].max_meals = -1;
		(*data)[i].philo_id = i + 1;
		(*data)[i].start_time = get_time();
		(*data)[i].last_ate = current_time(*data);
		i++;
	}
	if(create_forks(*data, forks) == -1)
	{
		free(data);
		return (-1);
	}
	return (0);	
}
