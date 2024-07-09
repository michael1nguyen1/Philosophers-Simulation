/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_fts.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linhnguy <linhnguy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 21:58:03 by linhnguy          #+#    #+#             */
/*   Updated: 2024/07/09 14:46:09 by linhnguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	get_time(void)
{
	struct timeval	time;
	int				seconds;
	int				useconds;
	int				total;

	gettimeofday(&time, NULL);
	seconds = time.tv_sec;
	useconds = time.tv_usec;
	total = seconds * 1000 + useconds / 1000.0;
	return (total);
}

int	current_time(t_philo *data)
{
	return (get_time() - data->start_time);
}

void	my_usleep(t_philo *data, int time)
{
	int	start;

	start = current_time(data);
	if (!check_mutex(data->dead, data))
		return ;
	while (current_time(data) - start < time)
	{
		if (!check_mutex(data->dead, data))
			return ;
		else if (current_time(data) - data->last_ate > data->die_time
			&& check_mutex(data->dead, data))
		{
			raise_dead_flag(data);
			pthread_mutex_lock(data->print);
			printf("%d %d died\n", current_time(data), data->philo_id);
			pthread_mutex_unlock(data->print);
			return ;
		}
		usleep(500);
	}
}
