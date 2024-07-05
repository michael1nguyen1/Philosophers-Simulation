#ifndef PHILO_H
#define PHILO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h> 

typedef struct s_philo
{
	int				philo;
	int				die_time;
	int				eat_time;
	int				sleep_time;
	int				max_meals;
	int             philo_id;
	int 			*alive;
	int             start_time;
	int 			meals_ate;
	int				last_ate;
	pthread_mutex_t *dead;
	pthread_mutex_t *left_fork;
	pthread_mutex_t *right_fork;
	pthread_mutex_t *print;
} t_philo;

typedef struct s_creeper
{
	int     death;
	t_philo *data;
} t_creeper;

int     ft_atoi(const char *str);
int		check_args(int argc, char **argv);
int		init_struct(int argc, char** argv, t_philo **data ,pthread_mutex_t **forks);

int		put_error_fd(int fd, char *str);
void	print_actions(t_philo *data, char *str);

int     check_mutex(pthread_mutex_t *check, t_philo *data);
int		destroy_mutex_array(pthread_mutex_t *forks, int amount);
int		create_rest_of_mutex(pthread_mutex_t *print, pthread_mutex_t *dead, t_philo **data);
void	raise_dead_flag(t_philo *data);

void	my_usleep(t_philo *data, int time);
int		current_time(t_philo *data);

int		create_forks(t_philo *data, pthread_mutex_t **forks);
void	assign_forks(t_philo *data, pthread_mutex_t *forks);
void	pick_up_forks(t_philo *data);
void	put_down_forks(t_philo *data);

void	thinking(t_philo *data);
void	go_to_sleep(t_philo *data);
void	eat(t_philo *data);
void	*lonely_philo(t_philo *data);

int		current_time(t_philo *data);
int		get_time();

#endif
