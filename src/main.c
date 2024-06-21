/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linhnguy <linhnguy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 13:13:44 by linhnguy          #+#    #+#             */
/*   Updated: 2024/06/21 21:04:58 by linhnguy         ###   ########.fr       */
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

int	destroy_mutex_array(pthread_mutex_t *forks, int amount)
{
	int	i;

	i = 0;
	while (i < amount)
	{
		pthread_mutex_destroy(&forks[i]);
		i++;
	}
	return (0);
}

int	get_time()
{
	struct timeval	time;
	int 			seconds;
	int			useconds;
	int				total;

	gettimeofday(&time, NULL);
	seconds = time.tv_sec;
	useconds = time.tv_usec;
	total = seconds * 1000 + useconds / 1000.0;
	return (total);
}

int	eat(t_philo *data)
{
	int	time;

	time = get_time();
	
	usleep(data->eat_time);
	return (0);
}

void *philo_life(void *arg)
{
	t_philo *data;

	data = (t_philo*)arg;
	while (data->alive)
	{
		eat(data);
		break;
	}
	return NULL;
}

int	simulation(t_philo *data)
{
	pthread_t thread[data[0].philo];
	int i;
	
	i = 0;
	while (i < data[0].philo)
	{
		if (pthread_create(&thread[i], NULL, &philo_life, data) != 0)
			return(put_error_fd(2, "thread failed\n"));
		i++;
	}
	i = 0;
	while (i < data[0].philo)
	{
		if(pthread_join(thread[i++],NULL) != 0)
			return(put_error_fd(2, "joined failed\n"));
	}
	return (0);
}

void	assign_forks(t_philo *data, pthread_mutex_t *forks)
{
	int i;

	i = 0;
	while (i < data->philo)
	{
		data[i].left_fork = &forks[i];
		if (i == data->philo - 1)
			data[i].right_fork = &forks[0];
		else
			data[i].right_fork = &forks[i + 1];
	}
}

int create_rest_of_mutex(pthread_mutex_t print, pthread_mutex_t dead, t_philo *data)
{
	int i;
	
	i = 0;
	if (pthread_mutex_init(&print, NULL) != 0)
	{
		return (put_error_fd(2, "mutex init failed\n"));
	}
	if (pthread_mutex_init(&dead, NULL) != 0)
	{
		pthread_mutex_destroy(&print);
		return (put_error_fd(2, "mutex init failed\n"));
	}
	while (i < data[0].philo)
	{
		data[i].dead = dead;
		data[i].print = print;
		i++;
	}
	return (0);
}

int	create_forks(t_philo *data, pthread_mutex_t *forks)
{
	int i;

	i = 0;
	forks = malloc(sizeof(pthread_mutex_t) * data->philo);
	if (!forks)
		return (put_error_fd(2, "malloc failed\n"));
	while (i < data->philo)
	{
		if (pthread_mutex_init(&forks[i], NULL) != 0)
		{
			destroy_mutex_array(forks, i);
			free(forks);
			return (put_error_fd(2, "mutex init failed\n"));
		}
		i++;
	}
	assign_forks(data, forks);
	return (0);
}

int	init_struct(int argc, char** argv, t_philo *data , pthread_mutex_t *forks)
{
	int i;

	int i = 0;
	data = malloc(sizeof(t_philo) * data->philo);
	if (!data)
		return (put_error_fd(2, "malloc failed in main\n"));
	while (i < data->philo)
	{	
		data[i].philo = ft_atoi(argv[1]);
		data[i].die_time = ft_atoi(argv[2]);
		data[i].eat_time = ft_atoi(argv[3]);
		data[i].sleep_time = ft_atoi(argv[4]);
		if (argc == 6)
			data[i].max_meals = ft_atoi(argv[5]);
		else
			data[i].max_meals = 0;
		data[i].alive = 1;
		data[i].meals_ate = 0;
		data[i].last_ate = 0;
		i++;
	}
	if (create_mutex(data, forks) == -1)
		return (-1);
	return (0);	
}

int main(int argc, char** argv)
{
	t_philo 		*data;
	pthread_mutex_t *forks;
	pthread_mutex_t dead;
	pthread_mutex_t print;
	
    if (argc == 5 || argc == 6)
	{
		if (check_args(argc, argv) == -1)
			return (put_error_fd(2, "Invalid arguments!\n"));
		if (init_struct(argc, argv, data, forks) == -1)
			return (-1);
		if (create_rest_of_mutex(print, dead, data) == -1)
		{
			destroy_mutex_array(forks, data->philo);
			free(data);
			return (-1);
		}
		if (simulation(&data) == -1)
			return (-1);
		return (destroy_mutex_array(forks, data->philo));
	}
    else
		return (put_error_fd(2, "Wrong number of ARGS!\n"));
	return (0);
}
