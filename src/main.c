/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linhnguy <linhnguy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 13:13:44 by linhnguy          #+#    #+#             */
/*   Updated: 2024/07/08 18:08:46 by linhnguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_struct(int argc, char **argv, t_philo **data, pthread_mutex_t **forks)
{
	*data = malloc(sizeof(t_philo) * ft_atoi(argv[1]));
	if (!*data)
		return (put_error_fd(2, "malloc failed in main\n"));
	convert_and_init(data, argv, argc);
	if (create_forks(*data, forks) == -1)
	{
		free(data);
		return (-1);
	}
	return (0);
}

int	create_threads(t_philo *data, pthread_t *creeper, int *death)
{
	int	i;

	i = 0;
	if (data->max_meals > 0 && data->philo > 1)
	{
		data->alive = death;
		if (pthread_create(creeper, NULL, &creeper_life, data) != 0)
			return (put_error_fd(2, "thread failed\n"));
	}
	while (i < data[0].philo)
	{
		pthread_mutex_lock(data[i].dead);
		data[i].alive = death;
		pthread_mutex_unlock(data[i].dead);
		if (pthread_create(&data[i].thread, NULL, &philo_life, &data[i]) != 0)
		{
			if (join_after_create_fail(data, i - 1) == -1)
				return (-1);
			return (put_error_fd(2, "thread failed\n"));
		}
		i++;
	}
	return (0);
}

int	simulation(t_philo *data)
{
	pthread_t	creeper;
	int			i;
	int			death;

	death = 1;
	i = 0;
	if (create_threads(data, &creeper, &death) == -1)
		return (-1);
	if (data->max_meals > 0 && data->philo > 1)
	{
		if (pthread_join(creeper, NULL) != 0)
			return (put_error_fd(2, "joined failed\n"));
	}
	while (i < data[0].philo)
	{
		if (pthread_join(data[i].thread, NULL) != 0)
			return (put_error_fd(2, "joined failed\n"));
		i++;
	}
	return (0);
}

int	create_forks(t_philo *data, pthread_mutex_t **forks)
{
	int	i;

	i = 0;
	*forks = malloc(sizeof(pthread_mutex_t) * data[0].philo);
	if (!*forks)
		return (put_error_fd(2, "malloc failed\n"));
	while (i < data[0].philo)
	{
		if (pthread_mutex_init(&(*forks)[i], NULL) != 0)
		{
			destroy_mutex_array(*forks, i);
			free(*forks);
			return (put_error_fd(2, "mutex init failed\n"));
		}
		i++;
	}
	assign_forks(data, *forks);
	return (0);
}

int	main(int argc, char **argv)
{
	t_philo			*data;
	pthread_mutex_t	*forks;
	pthread_mutex_t	dead;
	pthread_mutex_t	print;
	pthread_mutex_t	meal;

	if (argc == 5 || argc == 6)
	{
		if (check_args(argc, argv) == -1)
			return (put_error_fd(2, "Invalid arguments!\n"));
		if (init_struct(argc, argv, &data, &forks) == -1)
			return (-1);
		if (create_rest_of_mutex(&print, &dead, &meal, &data) == -1
			|| simulation(data) == -1)
		{
			clean_up(forks, data, &dead, &print);
			return (-1);
		}
		return (clean_up(forks, data, &dead, &print));
	}
	else
		return (put_error_fd(2, "Wrong number of ARGS!\n"));
	return (0);
}
