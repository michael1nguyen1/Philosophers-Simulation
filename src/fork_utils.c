/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linhnguy <linhnguy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 15:08:59 by linhnguy          #+#    #+#             */
/*   Updated: 2024/07/09 14:46:37 by linhnguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	pick_up_right_fork(t_philo *data)
{
	pthread_mutex_lock(data->left_fork);
	if (!check_mutex(data->dead, data))
	{
		pthread_mutex_unlock(data->left_fork);
		return (-1);
	}
	print_actions(data, "has taken a fork");
	return (0);
}

int	pick_up_forks(t_philo *data)
{
	if (pick_up_right_fork(data) == -1)
		return (-1);
	if (check_mutex(data->dead, data) && data->philo != 1)
	{
		pthread_mutex_lock(data->right_fork);
		if (!check_mutex(data->dead, data))
		{
			pthread_mutex_unlock(data->left_fork);
			pthread_mutex_unlock(data->right_fork);
			return (-1);
		}
	}
	else if (!check_mutex(data->dead, data) && data->philo != 1)
	{
		pthread_mutex_unlock(data->left_fork);
		return (-1);
	}
	if (data->philo != 1)
	{
		print_actions(data, "has taken a fork");
		print_actions(data, "is eating");
		data->last_ate = current_time(data);
	}
	return (0);
}

void	put_down_forks(t_philo *data)
{
	pthread_mutex_unlock(data->left_fork);
	if (data->philo != 1)
		pthread_mutex_unlock(data->right_fork);
}

void	assign_forks(t_philo *data, pthread_mutex_t *forks)
{
	int	i;

	i = 0;
	while (i < data[0].philo)
	{
		data[i].left_fork = &forks[i];
		if (data->philo == 1)
			data->right_fork = NULL;
		else if (i == data->philo - 1)
			data[i].right_fork = &forks[0];
		else
			data[i].right_fork = &forks[i + 1];
		i++;
	}
}

int	join_after_create_fail(t_philo *data, int i)
{
	raise_dead_flag(data);
	while (i > -1)
	{
		if (pthread_join(data[i].thread, NULL) != 0)
			return (put_error_fd(2, "joined failed\n"));
		i--;
	}
	return (0);
}
