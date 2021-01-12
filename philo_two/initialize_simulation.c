#include "includes.h"

int				take_forks(t_philsopher *p)
{
	sem_wait(p->booking);
	sem_wait(p->forks);
	print_time(p, TAKE_FORK);
	sem_wait(p->forks);
	print_time(p, TAKE_FORK);
	sem_post(p->booking);
	return (0);
}

static int		create_forks(t_philos_list *phs)
{
	sem_unlink("booking");
	sem_unlink("forks");
	sem_unlink("write_lock");
	sem_unlink("well_fed_num");
	sem_unlink("stop_lock");
	if ((phs->booking = sem_open("booking", S, 0644, 1)) == SF)
		return (return_error(SEM_ERROR));
	if ((phs->forks = sem_open("forks", S, 0644, phs->amount)) == SF)
		return (return_error(SEM_ERROR));
	if ((phs->write_lock = sem_open("write_lock", S, 0644, 1)) == SF)
		return (return_error(SEM_ERROR));
	if ((phs->well_fed_num = sem_open("well_fed_num", S, 0644, 0)) == SF)
		return (return_error(SEM_ERROR));
	if ((phs->stop_lock = sem_open("stop_lock", S, 0644, 0)) == SF)
		return (return_error(SEM_ERROR));
	return (0);
}

int				set_forks(t_philos_list *phs)
{
	int		i;
	char	buf[250];
	char	*pos;

	i = 0;
	if (create_forks(phs))
		return (1);
	while (i < phs->amount)
	{
		phs->philosophers[i]->start_time = &phs->start_time;
		phs->philosophers[i]->well_fed_num = phs->well_fed_num;
		phs->philosophers[i]->die_index = &phs->die_index;
		phs->philosophers[i]->win = &phs->win;
		phs->philosophers[i]->stop_lock = phs->stop_lock;
		phs->philosophers[i]->write_lock = phs->write_lock;
		phs->philosophers[i]->forks = phs->forks;
		phs->philosophers[i]->booking = phs->booking;
		phs->philosophers[i]->live_config = &phs->input_data;
		pos = ft_itoa_buf(i + 1, buf);
		*pos = '\0';
		sem_unlink(buf);
		phs->philosophers[i]->time_lock = sem_open(buf, S, 0644, 1);
		i++;
	}
	return (return_error(NULL));
}

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
