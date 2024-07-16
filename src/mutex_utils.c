/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linhnguy <linhnguy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 13:27:59 by linhnguy          #+#    #+#             */
/*   Updated: 2024/07/16 13:53:29 by linhnguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_stop(t_philo *data)
{
	pthread_mutex_lock(&data->dead);
	if (data->stop == 1)
	{
		pthread_mutex_unlock(&data->dead);
		return (1);
	}
	pthread_mutex_unlock(&data->dead);
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

int	create_rest_of_mutex(pthread_mutex_t *print,
		pthread_mutex_t *meal, t_philo **data)
{
	int	i;

	i = 0;
	if (pthread_mutex_init(print, NULL) != 0)
		return (put_error_fd(2, "mutex init failed\n"));
	if (pthread_mutex_init(meal, NULL) != 0)
	{
		pthread_mutex_destroy(print);
		return (put_error_fd(2, "mutex init failed\n"));
	}
	while (i < (*data)[0].philo)
	{
		(*data)[i].print = print;
		(*data)[i].meals = meal;
		if (pthread_mutex_init(&(*data)[i].dead, NULL) != 0)
		{
			pthread_mutex_destroy(print);
			pthread_mutex_destroy(meal);
			while (--i > -1)
				pthread_mutex_destroy(&(*data)[i].dead);
			return (put_error_fd(2, "mutex init failed\n"));
		}
		i++;
	}
	return (0);
}
