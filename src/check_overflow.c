/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_overflow.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linhnguy <linhnguy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 17:45:00 by linhnguy          #+#    #+#             */
/*   Updated: 2024/07/06 18:11:24 by linhnguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

int	ft_strncmp(const char *str1, const char *str2, size_t n)
{
	size_t	i;

	i = 0;
	if (n == 0)
		return (0);
	while (i < n && (str1[i] || str2[i]))
	{
		if (str1[i] != str2[i])
			return ((unsigned char) str1[i] - (unsigned char) str2[i]);
		i++;
	}
	return (0);
}


int is_greater_than_int_max(const char *str) 
{
	char	*max;
	int		len;
	int		max_len;
	
	max = "2147483647";
	max_len = ft_strlen (max);
	len = ft_strlen(str);
    if (len > max_len)
        return (1); 
    else if (len < max_len)
        return (0); 
    else 
	{
		if (ft_strncmp(str, max, 10) > 0)
        return (1);
    }
	return (0);
}

int	check_overflow(char **argv)
{
	int i;
	i = 1;
	while (argv[i])
	{
		if (is_greater_than_int_max(argv[i]))
			return (-1);
		i++;
	}
	return (0);
}
// int main(int argc, char **argv)
// {
// 	printf("%d\n", check_overflow(argv));
// }