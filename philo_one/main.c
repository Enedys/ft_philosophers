#include "includes.h"

static void	*monitor(void *philos)
{
	t_philos_list	*phs;

	phs = (t_philos_list *)philos;
	while (1)
	{
		pthread_mutex_lock(&phs->stop_lock);
		if (phs->die_index)
			return (0);
		if (phs->well_fed_num == phs->amount)
		{
			pthread_mutex_lock(&phs->write_lock);
			print_event(0, get_time(&phs->start_time), WELL_FED);
			return (0);
		}
		pthread_mutex_unlock(&phs->stop_lock);
		usleep(1000);
	}
	return (0);
}

void		*philo_observer(void *philo)
{
	t_philsopher	*p;
	int				win_cond;

	p = (t_philsopher *)philo;
	win_cond = (p->live_config->ctw == -1) ? 0 : 1;
	while (!stopping_criterion(p, &win_cond))
		usleep(1000);
	return (0);
}

int			start_thread(void *(*function)(void *), void *data,\
										pthread_mutex_t *lock)
{
	pthread_t	thread;

	if (pthread_create(&thread, NULL, function, data))
	{
		pthread_mutex_lock(lock);
		return (return_error(THREAD_ERROR));
	}
	pthread_detach(thread);
	return (0);
}

static int	run_simulation(t_philos_list *phs)
{
	int			i;
	pthread_t	thread;

	i = 0;
	gettimeofday(&phs->start_time, NULL);
	while (i < phs->amount)
	{
		if (start_thread(start_routine, phs->philosophers[i],\
										&phs->write_lock))
			return (1);
		usleep(10);
		i++;
	}
	if (pthread_create(&thread, NULL, monitor, phs) ||\
							pthread_join(thread, NULL))
	{
		pthread_mutex_lock(&phs->write_lock);
		return (return_error(THREAD_ERROR));
	}
	return (return_error(NULL));
}

int			main(int argc, char **argv)
{
	t_philos_list		phs;
	int					res;

	memset(&phs, 0, sizeof(t_philos_list));
	if (load_input_data(&phs.input_data, argc, argv))
		return (1);
	phs.amount = phs.input_data.amount;
	if (!(phs.philosophers = create_philosophers(phs.amount)))
		return (1);
	if (set_forks(&phs))
		return (clean_data(&phs));
	res = run_simulation(&phs);
	clean_data(&phs);
	return (res);
}
