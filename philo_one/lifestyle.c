#include "includes.h"

static int		take_fork(t_philsopher *p, int act)
{
	int				*handler;
	pthread_mutex_t	*fork;

	fork = (act == F) ? p->first_fork : p->second_fork;
	handler = (act == F) ? p->f_last_handler : p->s_last_handler;
	while (1)
	{
		pthread_mutex_lock(fork);
		if (*handler != p->num)
		{
			*handler = p->num;
			break ;
		}
		pthread_mutex_unlock(fork);
		usleep(1000);
	}
	print_time(p, TAKE_FORK);
	return (0);
}

static int		philo_sleep(t_philsopher *p)
{
	print_time(p, SLEEPING);
	usleep(1000 * p->live_config->tts);
	return (1);
}

static int		philo_eat(t_philsopher *p)
{
	pthread_mutex_lock(p->stage_lock);
	gettimeofday(&p->last_event, NULL);
	pthread_mutex_unlock(p->stage_lock);
	print_time(p, EATING);
	usleep(1000 * p->live_config->tte);
	pthread_mutex_lock(p->stage_lock);
	p->ctw++;
	pthread_mutex_unlock(p->stage_lock);
	pthread_mutex_unlock(p->second_fork);
	pthread_mutex_unlock(p->first_fork);
	return (1);
}

static int		philo_think(t_philsopher *p)
{
	print_time(p, THINKING);
	take_fork(p, F);
	take_fork(p, S);
	philo_eat(p);
	philo_sleep(p);
	return (1);
}

void			*start_routine(void *philosoph)
{
	t_philsopher	*p;

	p = (t_philsopher *)philosoph;
	gettimeofday(&p->last_event, NULL);
	if (start_thread(philo_observer, p, p->write_lock))
	{
		pthread_mutex_lock(p->stop_lock);
		*p->die_index = p->num;
		pthread_mutex_unlock(p->stop_lock);
		return (0);
	}
	while (philo_think(p))
		;
	return (0);
}
