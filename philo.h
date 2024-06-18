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
    int philo;
    int die_time;
    int eat_time;
    int sleep_time;
    int eat_count;
    int dead_flag;
    long last_eat;
    pthread_t thread;
} t_philo;

#endif
