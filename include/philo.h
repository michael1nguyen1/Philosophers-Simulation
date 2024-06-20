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
    int 			*philo_num;
    int 			dead_flag;
    int 			*meals_ate;
    int				*last_ate;
    pthread_mutex_t *forks;
	pthread_mutex_t print;
} t_philo;

int     ft_atoi(const char *str);

#endif
