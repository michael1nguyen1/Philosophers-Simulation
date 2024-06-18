/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linhnguy <linhnguy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 13:13:44 by linhnguy          #+#    #+#             */
/*   Updated: 2024/06/18 16:47:07 by linhnguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int put_error_fd(int fd, char *str)
{
	int len;
	
	while (str[len])
		len++;
	write(fd, str, len);
	return (-1);
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

void init_struct(int argc, char** argv, t_philo *data)
{
	data->philo = ft_atoi(argv[1]);
	data->die_time = ft_atoi(argv[2]);
	data->eat_time = ft_atoi(argv[3]);
	data->sleep_time = ft_atoi(argv[4]);
	if (argc == 6)
		data->eat_count = ft_atoi(argv[5]);
	else
		data->eat_count = 0;
	data->dead_flag = 0;
}

void start_simulation(t_philo *data)
{
	int i;
	
	i = 0;
	while (i < data->philo)
	{
		pthread_create(&data[i].thread, NULL, &philo_life, &data[i]);
		i++;
	}
}

int main(int argc, char** argv)
{
	t_philo data;
	pthread_mutex_t mutex;
    if (argc == 5 || argc == 6)
	{
		if (check_args(argc, argv) == -1)
			return (put_error_fd(2, "Invalid arguments!"));
		init_struct(argc, argv, &data);
		pthread_mutex_init(&mutex, NULL);
		start_simulation(&data);
		end_simulation(&data);
		pthread_mutex_destroy(&mutex);
	}
    else
		return (put_error_fd(2, "Wrong number of ARGS!"));
	return (0);
}