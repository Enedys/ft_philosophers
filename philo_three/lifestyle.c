#include "includes.h"

static void	*die_monitor(void *philo)
{
	t_philsopher	*p;

	p = (t_philsopher *)philo;
	while (1)
	{
		sem_wait(p->time_lock);
		if (get_time(&p->last_event) > p->live_config->ttd)
		{
			sem_wait(p->write_lock);
			print_event(p->num, get_time(p->start_time), DIE);
			sem_post(p->stop_lock);
			return (0);
		}
		sem_post(p->time_lock);
		usleep(1000);
	}
	return (0);
}

int			philo_sleep(t_philsopher *p)
{
	print_time(p, SLEEPING);
	usleep(1000 * p->live_config->tts);
	return (1);
}

int			philo_eat(t_philsopher *p)
{
	sem_wait(p->time_lock);
	gettimeofday(&p->last_event, NULL);
	sem_post(p->time_lock);
	print_time(p, EATING);
	usleep(1000 * p->live_config->tte);
	if (++p->ctw == p->live_config->ctw)
		sem_post(p->well_fed_num);
	sem_post(p->forks);
	sem_post(p->forks);
	return (1);
}

int			philo_think(t_philsopher *p)
{
	print_time(p, THINKING);
	take_forks(p);
	philo_eat(p);
	philo_sleep(p);
	return (1);
}

void		*start_routine(void *philosoph)
{
	t_philsopher	*p;

	p = (t_philsopher *)philosoph;
	gettimeofday(&p->last_event, NULL);
	if (start_thread(die_monitor, p, p->write_lock))
	{
		sem_post(p->stop_lock);
		exit(0);
	}
	while (1)
		philo_think(p);
	return (NULL);
}
