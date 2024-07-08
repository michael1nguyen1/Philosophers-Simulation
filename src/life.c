/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   life.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linhnguy <linhnguy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 16:51:20 by linhnguy          #+#    #+#             */
/*   Updated: 2024/07/08 17:10:11 by linhnguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*philo_life(void *arg)
{
	t_philo	*data;

	data = (t_philo *)arg;
	if (data->philo == 1)
		return (lonely_philo(data));
	if (data->philo_id % 2 == 0)
		usleep(500);
	print_actions(data, "is thinking");
	while (check_mutex(data->dead, data))
	{
		eat(data);
		if (!check_mutex(data->dead, data))
			break ;
		go_to_sleep(data);
		if (!check_mutex(data->dead, data))
			break ;
		thinking(data);
	}
	return (NULL);
}

void	*creeper_life(void *arg)
{
	t_philo	*data;
	int		i;

	data = (t_philo *)arg;
	i = 0;
	while (1)
	{
		if (!check_meals(&data[i].meals_ate, &data[i])
			&& check_mutex(data->dead, data))
			i--;
		if (i == data->philo - 1 || !check_mutex(data->dead, data))
		{
			raise_dead_flag(data);
			break ;
		}
		i++;
	}
	return (NULL);
}

int	check_meals(int *meals, t_philo *data)
{
	int	ret;

	ret = 0;
	pthread_mutex_lock(data->meals);
	if (*meals >= data->max_meals)
		ret = 1;
	pthread_mutex_unlock(data->meals);
	return (ret);
}

void	count_meals(t_philo *data)
{
	pthread_mutex_lock(data->meals);
	data->meals_ate++;
	pthread_mutex_unlock(data->meals);
}
