/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linhnguy <linhnguy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 13:13:44 by linhnguy          #+#    #+#             */
/*   Updated: 2024/06/19 22:24:20 by linhnguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	put_error_fd(int fd, char *str)
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

int	init_struct(int argc, char** argv, t_philo *data)
{
	data->philo = ft_atoi(argv[1]);
	data->die_time = ft_atoi(argv[2]);
	data->eat_time = ft_atoi(argv[3]);
	data->sleep_time = ft_atoi(argv[4]);
	if (argc == 6)
		data->eat_count = ft_atoi(argv[5]);
	else
		data->eat_count = 0;
	data->dead_flag = 1;
	if (pthread_mutex_init(&data->mutex, NULL) != 0)
		return (put_error_fd(2, "mutex init failed"));
	data->philo_count = 0;
	data->meals_ate = 0;
	data->last_ate = malloc(sizeof(int) * data->philo);
	if (!data->last_ate)
		return(put_error_fd(2, "malloc failed in init_struct\n"));
	data->forks = malloc(sizeof(pthread_mutex_t) * data->philo);
	if (!data->last_ate)
		return(put_error_fd(2, "malloc failed in init_struct\n"));
	return (0);	
}

double	get_time()
{
	struct timeval	time;
	long 			seconds;
	long			useconds;
	double			total;

	gettimeofday(&time, NULL);
	seconds = time.tv_sec;
	useconds = time.tv_usec;
	total = seconds * 1000 + useconds / 1000.0;
	return (total);
}

void sleep(t_philo *data)
{
	
}

void *philo_life(void *arg)
{
	t_philo *data;
	int		i;
	
	i = 0;
	data = (t_philo*)arg;
	while (data->dead_flag || data->meals_ate != data->eat_count)
	{
		if (i == data->philo - 1)
			i = 0;
		data->last_ate[i] = get_time();
		if (data->philo_count == data->philo)
			data->meals_ate++;
		i++;
	}
	return NULL;
}

int	simulation(t_philo *data)
{
	pthread_t thread[data->philo];
	int i;
	
	i = 0;
	while (i < data->philo)
	{
		data->philo_count = malloc(sizeof(int));
		if (!data->philo_count)
			return(put_error_fd(2, "malloc in simulation failed\n"));
		data->philo_count = i + 1;
		if (pthread_create(&thread[i++], NULL, &philo_life, data) != 0)
			return(put_error_fd(2, "thread failed\n"));
		free(data->philo_count);
		i++;
	}
	i = 0;
	while (i < data->philo)
	{
		if(pthread_join(thread[i++],NULL) != 0)
			return(put_error_fd(2, "joined failed\n"));
	}
	return (0);
}

int main(int argc, char** argv)
{
	t_philo data;
	
    if (argc == 5 || argc == 6)
	{
		if (check_args(argc, argv) == -1)
			return (put_error_fd(2, "Invalid arguments!\n"));
		if (init_struct(argc, argv, &data) == -1)
			return (-1);
		if (simulation(&data) == -1)
			return (-1);
		if (pthread_mutex_destroy(&data.mutex) != 0)
			return (put_error_fd(2, "mutex destroy failed\n"));
	}
    else
		return (put_error_fd(2, "Wrong number of ARGS!\n"));
	return (0);
}