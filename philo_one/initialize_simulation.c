#include "includes.h"

void			*clean_philosophers(t_philsopher **philos, int num)
{
	int	i;

	i = 0;
	if (!philos)
		return (NULL);
	while (i < num)
		free(philos[i++]);
	free(philos);
	return (NULL);
}

t_philsopher	**create_philosophers(int num)
{
	t_philsopher	**philosophers;
	int				i;

	i = 0;
	if (!(philosophers = (t_philsopher **)malloc(sizeof(t_philsopher *) * num)))
		return (return_error(ALLOCATION_ERROR) ? NULL : NULL);
	while (i < num)
	{
		if (!(philosophers[i] = (t_philsopher *)malloc(sizeof(t_philsopher))))
		{
			clean_philosophers(philosophers, ++i);
			return (return_error(ALLOCATION_ERROR) ? NULL : NULL);
		}
		memset(philosophers[i], 0, sizeof(t_philsopher));
		philosophers[i]->num = i + 1;
		i++;
	}
	return (philosophers);
}

static int		create_forks(t_philos_list *phs)
{
	int	i;
	int	res;

	i = 0;
	res = 0;
	if (!(phs->forks = (pthread_mutex_t *)malloc(\
			sizeof(pthread_mutex_t) * 2 * phs->amount)))
		return (return_error(ALLOCATION_ERROR));
	while (i < 2 * phs->amount)
		res |= pthread_mutex_init(&phs->forks[i++], NULL);
	res |= pthread_mutex_init(&phs->write_lock, NULL);
	res |= pthread_mutex_init(&phs->stop_lock, NULL);
	if (!(phs->forks_states = (int *)malloc(sizeof(int) * phs->amount)))
		return (return_error(ALLOCATION_ERROR));
	memset(phs->forks_states, 0, sizeof(int) * phs->amount);
	return (return_error(res == 0 ? NULL : MUTEX_ERROR));
}

int				set_forks(t_philos_list *phs)
{
	int	i;

	i = 0;
	if (create_forks(phs))
		return (1);
	while (i < phs->amount)
	{
		phs->philosophers[i]->start_time = &phs->start_time;
		phs->philosophers[i]->well_fed_num = &phs->well_fed_num;
		phs->philosophers[i]->die_index = &phs->die_index;
		phs->philosophers[i]->first_fork = &phs->forks[(i + i % 2) %\
														phs->amount];
		phs->philosophers[i]->second_fork = &phs->forks[(i + (i + 1) % 2) %\
														phs->amount];
		phs->philosophers[i]->f_last_handler = &phs->forks_states[(i + i % 2) %\
														phs->amount];
		phs->philosophers[i]->s_last_handler = &phs->forks_states[(i + (i + 1)\
														% 2) % phs->amount];
		phs->philosophers[i]->stage_lock = &phs->forks[phs->amount + i];
		phs->philosophers[i]->stop_lock = &phs->stop_lock;
		phs->philosophers[i]->write_lock = &phs->write_lock;
		phs->philosophers[i]->live_config = &phs->input_data;
		i++;
	}
	return (return_error(NULL));
}
