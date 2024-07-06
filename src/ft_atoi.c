/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linhnguy <linhnguy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 15:29:53 by linhnguy          #+#    #+#             */
/*   Updated: 2024/07/06 17:35:43 by linhnguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	ft_isspace(char s)
{
	if ((s >= 9 && s <= 13) || s == 32)
		return (1);
	return (0);
}

int	ft_isdigit(int n)
{
	if (n >= '0' && n <= '9')
		return (1);
	else
		return (0);
}

int	ft_atoi(const char *str)
{
	long	dest;
	long	i;
	long	neg;

	dest = 0;
	i = 0;
	neg = 1;
	while (ft_isspace(str[i]))
		i++;
	if ((str[i] == 45 || str[i] == 43))
	{
		if (str[i] == 45)
			neg = -1;
		i++;
	}
	while (ft_isdigit(str[i]))
	{
		dest = dest * 10 + str[i++] - '0';
		if (dest < 0 && neg == 1)
			return (-1);
		else if (dest < 0 && neg == -1)
			return (0);
	}
	return (dest * neg);
}
