/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_life.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linhnguy <linhnguy@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 21:46:25 by linhnguy          #+#    #+#             */
/*   Updated: 2024/08/30 15:39:25 by linhnguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	thinking(t_philo *data)
{
	print_actions(data, "is thinking");
}

void	go_to_sleep(t_philo *data)
{
	print_actions(data, "is sleeping");
	my_usleep(data, data->sleep_time);
}

void	eat(t_philo *data)
{
	if (!check_stop(data))
	{
		if (pick_up_forks(data) == -1)
			return ;
		data->last_ate = current_time(data);
		my_usleep(data, data->eat_time);
		count_meals(data);
		put_down_forks(data);
	}
	return ;
}

void	*lonely_philo(t_philo *data)
{
	pick_up_forks(data);
	my_usleep(data, data->die_time);
	put_down_forks(data);
	pthread_mutex_lock(data->print);
	printf("%d philosopher %d died\n", current_time(data), data->philo_id);
	pthread_mutex_unlock(data->print);
	return (NULL);
}

void	raise_dead_flag(t_philo *data)
{
	pthread_mutex_lock(&data->dead);
	data->stop = 1;
	pthread_mutex_unlock(&data->dead);
}
