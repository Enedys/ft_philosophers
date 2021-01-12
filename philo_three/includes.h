#ifndef INCLUDES_H
# define INCLUDES_H
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <sys/time.h>
# include <pthread.h>
# include <fcntl.h>
# include <semaphore.h>
# include <signal.h>

# define INVALID_ARGS			"Invalid number of arguments.\n"
# define BAD_ARG				"Bad input arguments.\n"
# define ALLOCATION_ERROR		"Bad allocation.\n"
# define CORRECT_OUTPUT			"Corret output.\n"
# define SEM_ERROR				"Semaphore was not created.\n"
# define THREAD_ERROR			"Thread was not created.\n"
# define PROCESS_ERROR			"Process was not created.\n"

# define TAKE_FORK				" has taken a fork\n"
# define EATING					" is eating\n"
# define SLEEPING				" is sleeping\n"
# define THINKING				" is thinking\n"
# define DIE					" died\n"
# define WELL_FED				" All philosophers are well-fed.\n"

# define S						O_CREAT | O_EXCL
# define SF						SEM_FAILED

/*
** phil_num - number of philosophers
** ttd - time to die
** tte - time to eat
** tts - time to sleep
** ctw - cycles to win (number of times each philosopher must eat
**										 before abort simulation)
** S - semaphore creation parameters
** SF - semaphore failed define
*/

char				*g_strerror;

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
	struct timeval		last_event;
	struct timeval		*start_time;
	t_input_data		*live_config;
	sem_t				*write_lock;
	sem_t				*booking;
	sem_t				*forks;
	sem_t				*well_fed_num;
	sem_t				*stop_lock;
	sem_t				*time_lock;
	int					num;
	int					*die_index;
	int					*win;
	int					ctw;
}					t_philsopher;

typedef struct		s_philos_list
{
	struct timeval		start_time;
	t_input_data		input_data;
	t_philsopher		**philosophers;
	pid_t				*pids;
	sem_t				*forks;
	sem_t				*booking;
	sem_t				*write_lock;
	sem_t				*well_fed_num;
	sem_t				*stop_lock;
	int					amount;
	int					die_index;
	int					win;
}					t_philos_list;

/*
** Time managment
*/
int					print_time(t_philsopher *p, char *s);
int					get_time(struct timeval *start_time);
int					print_event(int num, int time, char *s);

/*
** Libft functions
*/
size_t				ft_strlcpy(char *d, const char *src, size_t n);
size_t				ft_strlen(const char *s);
char				*ft_itoa_buf(int n, char *p);
int					ft_atoi(char *str);

/*
** Load and analysis input parameters
*/
int					load_input_data(t_input_data *input_data, int argc,\
									char **argv);

/*
** Thread managment
*/
int					kill_process(pid_t *pids, int num, char *msg);
int					start_thread(void *(*function)(void *), void *data,\
								sem_t *lock);
int					take_forks(t_philsopher *p);
void				*start_routine(void *philosoph);

/*
** Initialize simulation
*/
t_philsopher		**create_philosophers(int num);
int					set_forks(t_philos_list *philos);

/*
** Error managment and exit processing
*/
void				*clean_philosophers(t_philsopher **philos, int num);
int					return_error(char *strerror);
int					clean_data(t_philos_list *philos);

#endif
