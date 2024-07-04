/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_funcs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linhnguy <linhnguy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 21:58:03 by linhnguy          #+#    #+#             */
/*   Updated: 2024/07/04 21:58:22 by linhnguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	get_time()
{
	struct timeval	time;
	int 			seconds;
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
