/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linhnguy <linhnguy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 13:27:59 by linhnguy          #+#    #+#             */
/*   Updated: 2024/07/02 14:10:28 by linhnguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int check_mutex(pthread_mutex_t check, t_philo *data)
{
    int ret;
    pthread_mutex_lock(&check);
        ret = *data->alive;
    pthread_mutex_unlock(&check);
    // printf("ret is %i\n", ret);
        return (ret);
}