/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linhnguy <linhnguy@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 21:47:32 by linhnguy          #+#    #+#             */
/*   Updated: 2024/08/30 15:39:56 by linhnguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	put_error_fd(int fd, char *str)
{
	int	len;

	len = 0;
	while (str[len])
		len++;
	write(fd, str, len);
	return (-1);
}

void	print_actions(t_philo *data, char *str)
{
	pthread_mutex_lock(data->print);
	if (!check_stop(data))
		printf("%d philosopher %d %s\n", current_time(data), data->philo_id, str);
	pthread_mutex_unlock(data->print);
}

int	check_args(int argc, char **argv)
{
	int	i;
	int	j;

	i = 1;
	if ((argv[5] && ft_atoi(argv[5]) == 0) || check_overflow(argv) == -1
		|| ft_atoi(argv[1]) == 0 || ft_atoi(argv[2]) == 0
		|| ft_atoi(argv[3]) == 0 || ft_atoi(argv[4]) == 0)
		return (-1);
	while (i < argc)
	{
		j = 0;
		if (!argv[i][0])
			return (-1);
		while (argv[i][j])
		{
			if (argv[i][0] != '+' && (argv[i][j] < '0' || argv[i][j] > '9'))
				return (-1);
			j++;
		}
		i++;
	}
	return (0);
}

void	convert_and_init(t_philo **data, char **argv, int argc)
{
	int	i;

	i = 0;
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
		(*data)[i].stop = 0;
		(*data)[i].start_time = get_time();
		(*data)[i].last_ate = current_time(*data);
		i++;
	}
}

int	clean_up(pthread_mutex_t *forks, t_philo *data, pthread_mutex_t *print)
{
	int	i;

	i = 0;
	destroy_mutex_array(forks, data->philo);
	if (forks)
		free (forks);
	pthread_mutex_destroy(data->meals);
	pthread_mutex_destroy(print);
	while (i < data->philo)
	{
		pthread_mutex_destroy(&data[i].dead);
		i++;
	}
	if (data)
		free (data);
	return (0);
}
