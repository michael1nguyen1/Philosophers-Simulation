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
    pthread_mutex_t dead;
    pthread_mutex_t *left_fork;
    pthread_mutex_t *right_fork;
	pthread_mutex_t print;
} t_philo;

typedef struct s_creeper
{
    int     death;
    t_philo *data;
} t_creeper;

int     ft_atoi(const char *str);

#endif
