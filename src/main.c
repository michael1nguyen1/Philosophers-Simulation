/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linhnguy <linhnguy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 13:13:44 by linhnguy          #+#    #+#             */
/*   Updated: 2024/07/05 19:00:12 by linhnguy         ###   ########.fr       */
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
	// printf("philo %d exited life\n", data->philo_id);
	return (NULL);
}

void	*creeper_life(void *arg)
{
	t_philo	*data;

	data = (t_philo *)arg;
	while (1)
	{
		if (data[0].meals_ate == data->max_meals
			|| !check_mutex(data->dead, data))
		{
			raise_dead_flag (data);
			break ;
		}
	}
	return (NULL);
}

int	simulation(t_philo *data)
{
	pthread_t	thread[data[0].philo];
	pthread_t	creeper;
	int			i;
	int			death;

	death = 1;
	i = 0;
	data->alive = &death;
	if (pthread_create(&creeper, NULL, &creeper_life, data) != 0)
		return (put_error_fd(2, "thread failed\n"));
	while (i < data[0].philo)
	{
		data[i].alive = &death;
		if (pthread_create(&thread[i], NULL, &philo_life, &data[i]) != 0)
			return (put_error_fd(2, "thread failed\n"));
		i++;
	}
	i = 0;
	if (pthread_join(creeper, NULL) != 0)
		return (put_error_fd(2, "joined failed\n"));
	while (i < data[0].philo)
	{
		if (pthread_join(thread[i++], NULL) != 0)
			return (put_error_fd(2, "joined failed\n"));
	}
	return (0);
}

int	create_forks(t_philo *data, pthread_mutex_t **forks)
{
	int	i;

	i = 0;
	*forks = malloc(sizeof(pthread_mutex_t) * data[0].philo);
	if (!*forks)
		return (put_error_fd(2, "malloc failed\n"));
	while (i < data[0].philo)
	{
		if (pthread_mutex_init(&(*forks)[i], NULL) != 0)
		{
			destroy_mutex_array(*forks, i);
			free(*forks);
			return (put_error_fd(2, "mutex init failed\n"));
		}
		i++;
	}
	assign_forks(data, *forks);
	return (0);
}

int	main(int argc, char **argv)
{
	t_philo			*data;
	pthread_mutex_t	*forks;
	pthread_mutex_t	dead;
	pthread_mutex_t	print;

	if (argc == 5 || argc == 6)
	{
		if (check_args(argc, argv) == -1)
			return (put_error_fd(2, "Invalid arguments!\n"));
		if (init_struct(argc, argv, &data, &forks) == -1)
			return (-1);
		if (create_rest_of_mutex(&print, &dead, &data) == -1
			|| simulation(data) == -1)
		{
			clean_up(forks, data);
			return (-1);
		}
		return (clean_up(forks, data));
	}
	else
		return (put_error_fd(2, "Wrong number of ARGS!\n"));
	return (0);
}
