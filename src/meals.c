/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   meals.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linhnguy <linhnguy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 16:51:20 by linhnguy          #+#    #+#             */
/*   Updated: 2024/07/07 17:14:38 by linhnguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_meals(int *meals, t_philo *data)
{
	int ret;

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