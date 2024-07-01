/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linhnguy <linhnguy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 13:13:44 by linhnguy          #+#    #+#             */
/*   Updated: 2024/07/01 20:42:09 by linhnguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	put_error_fd(int fd, char *str)
{
	int len;
	
	while (str[len])
		len++;
	write(fd, str, len);
	return (-1);
}

int	check_args(int argc, char **argv)
{
	int i;
	int j;

	i = 1;
	while (i < argc)
	{
		j = 0;
		while (argv[i][j])
		{
			if (argv[i][j] != '+' && (argv[i][j] < '0' || argv[i][j] > '9'))
				return (-1);
			j++;
		}
		i++;
	}
	return (0);
}

int	destroy_mutex_array(pthread_mutex_t *forks, int amount)
{
	int	i;

	i = 0;
	while (i < amount)
	{
		pthread_mutex_destroy(&forks[i]);
		i++;
	}
	return (0);
}

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

int	go_to_sleep(t_philo *data)
{
	pthread_mutex_lock(&data->print);
	printf("%d %d is sleeping\n", get_time() - data->start_time, data->philo_id);
	usleep(data->sleep_time);
	printf("%d %d is thinking\n", get_time() - data->start_time, data->philo_id);
	pthread_mutex_unlock(&data->print);
	return (0);
}

void	print_actions(t_philo *data, char *str)
{
	pthread_mutex_lock(&data->print);
	printf("%d %d %s\n", get_time() - data->start_time, data->philo_id, str);
	pthread_mutex_unlock(&data->print);
}

int	eat(t_philo *data)
{
	pthread_mutex_lock(data->left_fork);
	print_actions(data, "has taken a left fork");
	pthread_mutex_lock(data->right_fork);
	print_actions(data, "has taken a right fork");
	print_actions(data, "is eating");
	printf("first %d\n", get_time() - data->last_ate);
	// printf("first . 5 %d\n", (*data)->last_ate);
	// printf("second %d\n", (*data)->die_time);
	if (data->max_meals > 0 && data->meals_ate == data->max_meals &&
		*data->alive == 1)
	{
		pthread_mutex_lock(&data->dead);
		data->alive = 0;
		pthread_mutex_unlock(&data->dead);
		pthread_mutex_unlock(data->left_fork);
		pthread_mutex_unlock(data->right_fork);
		print_actions(data, "is full");
		return (0);
	}
	else if (get_time() - data->last_ate > data->die_time &&
		*data->alive == 1)
	{
	printf("Here\n");
		usleep(get_time() - data->last_ate);
		pthread_mutex_lock(&data->dead);
		data->alive = 0;
		pthread_mutex_unlock(&data->dead);
		pthread_mutex_unlock(data->left_fork);
		pthread_mutex_unlock(data->right_fork);
		print_actions(data, "died");
		return (0);
	}
	else
		usleep(data->eat_time);
	pthread_mutex_unlock(data->left_fork);
	pthread_mutex_unlock(data->right_fork);
	data->last_ate = get_time() - data->start_time;
	// printf("last ate is %i\n", (*data)->last_ate);
	data->meals_ate++; //check 6th arg
	printf("philo %d is done eating\n", data->philo_id);
	pthread_mutex_unlock(&data->print);
	return (0);
}

void *philo_life(void *arg)
{
	t_philo *data;
	data = (t_philo*)arg;
	if (data->philo_id % 2 == 0)
		usleep(150);
	pthread_mutex_lock(&data->dead);
	while (data->alive)
	{
		if (data->alive == 0)
		{
			pthread_mutex_unlock(&data->dead);
			break;
		}
		pthread_mutex_unlock(&data->dead);
		eat(data);
		pthread_mutex_lock(&data->dead);
		if (data->alive == 0)
		{
			printf("Here2\n");
			pthread_mutex_unlock(&data->dead);
			break;
		}
		pthread_mutex_unlock(&data->dead);
		go_to_sleep(data);
	}
	printf("philo %d exited life\n", data->philo_id);
	return NULL;
}

// void	*creeper_life(void *arg)
// {
// 	int	i;
// 	int	printed;
// 	int count;

// 	t_philo **data;
// 	data = (t_philo**)arg;
// 	i = 0;
// 	printed = 0;
// 	count = 1;
// 	printf("CREEPER is alive\n");
// 	while (1)
// 	{
// 		if (i + 1 == (*data)[0].philo)
// 			i = 0;
// 		if ((*data)[i].alive == 0)
// 		{
// 			(*data)[i].alive = 1;
// 			count++;
// 			// printf("count is %d\n", count);
// 			if (printed == 0)
// 			{
// 				printed = 1;
// 				print_actions(&(*data)[i++], "died");
// 				continue;
// 			}
// 		// pthread_mutex_lock((*data)[i].left_fork);
// 		}
// 		i++;
// 		if (count == (*data)[0].philo)
// 			break;
// 	}
// 	// pthread_mutex_unlock((*data)->left_fork);
// 	return (NULL);
// }

int	simulation(t_philo *data)
{
	pthread_t thread[data[0].philo];
	// pthread_t creeper;
	int i;
	int	death;
	
	death = 1;
	i = 0;
	// if (pthread_create(&creeper, NULL, &creeper_life, data) != 0)
	// 	return(put_error_fd(2, "thread failed\n"));
	while (i < data[0].philo)
	{
		data[i].alive = &death;
		if (pthread_create(&thread[i], NULL, &philo_life, &data[i]) != 0)
			return(put_error_fd(2, "thread failed\n"));
		i++;
	}
	i = 0;
	// if(pthread_join(creeper, NULL) != 0)
	// 	return(put_error_fd(2, "joined failed\n"));
	while (i < data[0].philo)
	{
		if(pthread_join(thread[i++],NULL) != 0)
			return(put_error_fd(2, "joined failed\n"));
	}
	return (0);
}

void	assign_forks(t_philo *data, pthread_mutex_t *forks)
{
	int i;

	i = 0;
	while (i < data[0].philo)
	{
		data[i].left_fork = &forks[i];
		if (i == data->philo - 1)
			data[i].right_fork = &forks[0];
		else
			data[i].right_fork = &forks[i + 1];
		i++;
	}
}

int create_rest_of_mutex(pthread_mutex_t print, pthread_mutex_t dead, t_philo **data)
{
	int i;
	
	i = 0;
	if (pthread_mutex_init(&print, NULL) != 0)
	{
		return (put_error_fd(2, "mutex init failed\n"));
	}
	if (pthread_mutex_init(&dead, NULL) != 0)
	{
		pthread_mutex_destroy(&print);
		return (put_error_fd(2, "mutex init failed\n"));
	}
	while (i < (*data)[0].philo)
	{
		(*data)[i].dead = dead;
		(*data)[i].print = print;
		i++;
	}
	return (0);
}

int	create_forks(t_philo *data, pthread_mutex_t **forks)
{
	int i;

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

int	init_struct(int argc, char** argv, t_philo **data , pthread_mutex_t **forks)
{
	int i;

	i = 0;
	*data = malloc(sizeof(t_philo) * ft_atoi(argv[1]));
	if (!*data)
		return (put_error_fd(2, "malloc failed in main\n"));
	while (i < ft_atoi(argv[1]))
	{	
		(*data)[i].philo = ft_atoi(argv[1]);
		(*data)[i].die_time = ft_atoi(argv[2]);
		(*data)[i].eat_time = ft_atoi(argv[3]);
		(*data)[i].sleep_time = ft_atoi(argv[4]);
		if (argc == 6)
			(*data)[i].max_meals = ft_atoi(argv[5]);
		else
			(*data)[i].max_meals = 0;
		(*data)[i].philo_id = i + 1;
		(*data)[i].start_time = get_time();
		(*data)[i].last_ate = get_time();
		i++;
	}
	if(create_forks(*data, forks) == -1)
	{
		free(data);
		return (-1);
	}
	return (0);	
}

int main(int argc, char** argv)
{
	t_philo 		*data;
	pthread_mutex_t *forks;
	pthread_mutex_t dead;
	pthread_mutex_t print;

    if (argc == 5 || argc == 6)
	{
		if (check_args(argc, argv) == -1)
			return (put_error_fd(2, "Invalid arguments!\n"));
		if (init_struct(argc, argv, &data, &forks) == -1)
			return (-1);
		if (create_rest_of_mutex(print, dead, &data) == -1)
		{
			destroy_mutex_array(forks, data->philo);
			free(data);
			return (-1);
		}
		if (simulation(data) == -1)
			return (-1);
		return (destroy_mutex_array(forks, data->philo));
	}
    else
		return (put_error_fd(2, "Wrong number of ARGS!\n"));
	return (0);
}
