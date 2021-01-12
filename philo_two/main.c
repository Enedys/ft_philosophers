#include "includes.h"

static void	*finish_monitor(void *philos)
{
	t_philos_list	*phs;

	phs = (t_philos_list *)philos;
	sem_wait(phs->stop_lock);
	usleep((phs->input_data.min_gap + 5) * 1000);
	return (0);
}

static void	*meal_monitor(void *philos)
{
	t_philos_list	*phs;
	int				i;

	i = 0;
	phs = (t_philos_list *)philos;
	while (i++ < phs->amount)
		sem_wait(phs->well_fed_num);
	sem_wait(phs->write_lock);
	print_event(0, get_time(&phs->start_time), WELL_FED);
	sem_post(phs->stop_lock);
	return (0);
}

int			start_thread(void *(*function)(void *), void *data, sem_t *lock)
{
	pthread_t	thread;

	if (pthread_create(&thread, NULL, function, data))
	{
		sem_wait(lock);
		return (return_error(THREAD_ERROR));
	}
	pthread_detach(thread);
	return (0);
}

static int	run_simulation(t_philos_list *phs)
{
	int			i;

	i = 0;
	if (start_thread(meal_monitor, phs, phs->write_lock))
		return (1);
	gettimeofday(&phs->start_time, NULL);
	while (i < phs->amount)
	{
		if (start_thread(start_routine, phs->philosophers[i],\
						phs->write_lock))
			return (1);
		usleep(10);
		i++;
	}
	finish_monitor(phs);
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
