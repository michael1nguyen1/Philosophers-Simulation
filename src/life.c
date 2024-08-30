/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   life.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linhnguy <linhnguy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 16:51:20 by linhnguy          #+#    #+#             */
/*   Updated: 2024/07/25 16:47:12 by linhnguy         ###   ########.fr       */
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
		usleep(5000);
	print_actions(data, "is thinking");
	while (!check_stop(data))
	{
		eat(data);
		if (check_stop(data))
			break ;
		go_to_sleep(data);
		if (check_stop(data))
			break ;
		thinking(data);
	}
	return (NULL);
}

void	end_everyone(t_philo *data)
{
	int		j;

	j = 0;
	while (j < data->philo)
	{
		raise_dead_flag(&data[j]);
		j++;
	}
}

void	*creeper_life(void *arg)
{
	t_philo	*data;
	int		i;
	int		count;

	data = (t_philo *)arg;
	count = 0;
	i = 0;
	while (1)
	{
		if (current_time(data) - data[i].last_ate > data->die_time)
		{
			print_actions(data, "died");
			end_everyone(data);
			break ;
		}
		if (philo_full(&data[i]))
		{
			count++;
			if (count == data->philo)
				end_everyone(data);
		}
		if (++i == data->philo)
			i = 0;
	}
	return (NULL);
}

int	philo_full(t_philo *data)
{
	int	ret;

	ret = 0;
	pthread_mutex_lock(data->meals);
	if (data->max_meals == 0)
	{
		ret = 1;
		data->max_meals--;
	}
	pthread_mutex_unlock(data->meals);
	return (ret);
}

void	count_meals(t_philo *data)
{
	pthread_mutex_lock(data->meals);
	if (data->meals > 0)
		data->max_meals--;
	pthread_mutex_unlock(data->meals);
}
