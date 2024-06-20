/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linhnguy <linhnguy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 13:13:44 by linhnguy          #+#    #+#             */
/*   Updated: 2024/06/20 23:29:59 by linhnguy         ###   ########.fr       */
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

void	free_mallocs(t_philo *data)
{
	if (data->last_ate)
		free (data->last_ate);
	if (data->meals_ate)
		free (data->meals_ate);
	if (data->forks)
		free (data->forks);
}

int	destroy_mutex(t_philo *data, int amount)
{
	int	i;

	i = 0;
	while (i < amount)
	{
		if (pthread_mutex_destroy(&data->forks[i]) != 0)
			return (put_error_fd(2, "mutex destroy failed\n"));
		i++;
	}
	return (0);
}

int init_mutex(t_philo *data)
{
	int	i;

	i = 0;
	while (i < data->philo)
	{	
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
			return (put_error_fd(2, "mutex init failed"));
		i++;
	}
	if (pthread_mutex_init(&data->print, NULL) != 0)
	{
		destroy_mutex(data, data->philo);
		return (put_error_fd(2, "mutex init failed"));
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
		data->max_meals = ft_atoi(argv[5]);
	else
		data->max_meals = 0;
	data->philo_num = 0;
	data->dead_flag = 1;
	data->meals_ate = malloc(sizeof(int) * data->philo);
	data->last_ate = malloc(sizeof(int) * data->philo);
	data->forks = malloc(sizeof(pthread_mutex_t) * data->philo);
	if (!data->last_ate || !data->meals_ate || !data->forks)
	{
		free_mallocs(data);
		return(put_error_fd(2, "malloc failed in init_struct\n"));
	}
	if (init_mutex(data) == -1)
		return (-1);
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
	if (time - data->last_ate[*data->philo_num] > data->die_time)
	{
		printf("Philo %d has died\n", *data->philo_num + 1);
		data->dead_flag = 0;
		return (1);
	}
	printf("Philo num is %d is\n", *data->philo_num);
	printf("Philo %d is eating\n", *data->philo_num + 1);
	usleep(data->eat_time);
	return (0);
}

void *philo_life(void *arg)
{
	t_philo *data;

	printf("philo life\n");
	data = (t_philo*)arg;
	while (data->dead_flag)
	{
		data->last_ate[*data->philo_num] = get_time();
		eat(data);
		// break;
	}
	free(data->philo_num);
	return NULL;
}

int	simulation(t_philo *data)
{
	pthread_t thread[data->philo];
	int i;
	
	i = 0;
	while (i < data->philo)
	{
		data->philo_num = malloc(sizeof(int));
		if (!data->philo_num)
			return(put_error_fd(2, "malloc in simulation failed\n"));
		*data->philo_num = i;
		printf("philo num in sim%d\n", *data->philo_num);
		if (pthread_create(&thread[i], NULL, &philo_life, data) != 0)
			return(put_error_fd(2, "thread failed\n"));
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
//test these things
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
		return (destroy_mutex(&data, data.philo));
	}
    else
		return (put_error_fd(2, "Wrong number of ARGS!\n"));
	return (0);
}