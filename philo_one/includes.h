#ifndef INCLUDES_H
# define INCLUDES_H
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <sys/time.h>
# include <pthread.h>

# define INVALID_ARGS			"Invalid number of arguments.\n"
# define BAD_ARG				"Bad input arguments.\n"
# define ALLOCATION_ERROR		"Bad allocation.\n"
# define CORRECT_OUTPUT			"Corret output.\n"
# define MUTEX_ERROR			"Mutex was not created.\n"
# define THREAD_ERROR			"Thread was not created.\n"

# define TAKE_FORK				" has taken a fork\n"
# define EATING					" is eating\n"
# define SLEEPING				" is sleeping\n"
# define THINKING				" is thinking\n"
# define DIE					" died\n"
# define WELL_FED				" All philosophers are well-fed.\n"
# define F						0
# define S						1

# define STAGE_EAT

/*
** phil_num - number of philosophers
** ttd - time to die
** tte - time to eat
** tts - time to sleep
** ctw - cycles to win (number of times each philosopher must eat
**										 before abort simulation)
*/

char		*g_strerror;

typedef struct		s_input_data
{
	int					amount;
	int					min_gap;
	int					ttd;
	int					tte;
	int					tts;
	int					ctw;
}					t_input_data;

typedef struct		s_philosopher
{
	t_input_data		*live_config;
	pthread_mutex_t		*write_lock;
	pthread_mutex_t		*first_fork;
	pthread_mutex_t		*second_fork;
	pthread_mutex_t		*stage_lock;
	pthread_mutex_t		*stop_lock;
	struct timeval		last_event;
	struct timeval		*start_time;
	int					*f_last_handler;
	int					*s_last_handler;
	int					num;
	int					*well_fed_num;
	int					*die_index;
	int					ctw;
}					t_philsopher;

typedef struct		s_philos_list
{
	pthread_mutex_t		*forks;
	pthread_mutex_t		write_lock;
	pthread_mutex_t		stop_lock;
	struct timeval		start_time;
	t_input_data		input_data;
	t_philsopher		**philosophers;
	int					amount;
	int					*forks_states;
	int					well_fed_num;
	int					die_index;
}					t_philos_list;

/*
** Time managment
*/
int					get_time(struct timeval *start_time);
int					print_event(int num, int time, char *s);
int					print_time(t_philsopher *p, char *s);

/*
** Libft functions
*/
size_t				ft_strlen(const char *s);
size_t				ft_strlcpy(char *d, const char *src, size_t n);
char				*ft_itoa_buf(int n, char *p);
int					ft_atoi(char *str);

/*
** Load and analysis input parameters
*/
int					load_input_data(t_input_data *input_data,\
										int argc, char **argv);

/*
** Thread managment
*/
int					start_thread(void *(*function)(void *), void *data,\
												pthread_mutex_t *lock);
void				*philo_observer(void *philo);
int					stopping_criterion(t_philsopher *p, int *flag);
void				*start_routine(void *philosoph);

/*
** Initialize simulation
*/
t_philsopher		**create_philosophers(int num);
int					set_forks(t_philos_list *philos);

/*
** Error managment and exit processing
*/
int					return_error(char *strerror);
void				*clean_philosophers(t_philsopher **philos, int num);
int					clean_data(t_philos_list *philos);

#endif
