/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linhnguy <linhnguy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 13:27:59 by linhnguy          #+#    #+#             */
/*   Updated: 2024/07/08 16:37:01 by linhnguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_mutex(pthread_mutex_t *check, t_philo *data)
{
	int	ret;

	pthread_mutex_lock(check);
	ret = *data->alive;
	pthread_mutex_unlock(check);
	return (ret);
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

int	create_rest_of_mutex(pthread_mutex_t *print,
	pthread_mutex_t *dead, pthread_mutex_t *meal, t_philo **data)
{
	int	i;

	i = 0;
	if (pthread_mutex_init(print, NULL) != 0)
		return (put_error_fd(2, "mutex init failed\n"));
	if (pthread_mutex_init(dead, NULL) != 0)
	{
		pthread_mutex_destroy(print);
		return (put_error_fd(2, "mutex init failed\n"));
	}
	if (pthread_mutex_init(meal, NULL) != 0)
	{
		pthread_mutex_destroy(print);
		pthread_mutex_destroy(dead);
		return (put_error_fd(2, "mutex init failed\n"));
	}
	while (i < (*data)[0].philo)
	{
		(*data)[i].dead = dead;
		(*data)[i].print = print;
		(*data)[i].meals = meal;
		i++;
	}
	return (0);
}

int	create_threads(t_philo *data, pthread_t *creeper, int *death)
{
	int			i;

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
			raise_dead_flag(data);
			return (put_error_fd(2, "thread failed\n"));
		}
		i++;
	}
	return (0);
}
