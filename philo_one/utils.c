#include "includes.h"

int		print_time(t_philsopher *p, char *s)
{
	int		res;
	int		time;

	res = 0;
	res = pthread_mutex_lock(p->write_lock);
	time = get_time(p->start_time);
	if (time == -1)
		return (1);
	if (print_event(p->num, time, s) == -1)
		res |= 1;
	res |= pthread_mutex_unlock(p->write_lock);
	return (res);
}

int		print_event(int num, int time, char *s)
{
	char			buf[256];
	char			*tmp;

	tmp = ft_itoa_buf(time, buf);
	*tmp++ = '\t';
	if (num > 0)
		tmp = ft_itoa_buf(num, tmp);
	ft_strlcpy(tmp, s, -1);
	return (write(STDOUT_FILENO, buf, ft_strlen(buf)));
}

int		get_time(struct timeval *start_time)
{
	struct timeval	current_time;

	if (gettimeofday(&current_time, NULL))
		return (-1);
	return ((current_time.tv_sec - start_time->tv_sec) * 1000 +\
			(current_time.tv_usec - start_time->tv_usec) / 1000);
}

int		clean_data(t_philos_list *phs)
{
	int	i;

	i = 0;
	usleep((phs->input_data.min_gap + 5) * 1000);
	while (phs->forks && i < 2 * phs->amount)
		pthread_mutex_destroy(&phs->forks[i++]);
	pthread_mutex_destroy(&phs->write_lock);
	pthread_mutex_destroy(&phs->stop_lock);
	free(phs->forks);
	free(phs->forks_states);
	clean_philosophers(phs->philosophers, phs->amount);
	return (1);
}

int		stopping_criterion(t_philsopher *p, int *flag)
{
	pthread_mutex_lock(p->stage_lock);
	if (get_time(&p->last_event) > p->live_config->ttd)
	{
		pthread_mutex_lock(p->write_lock);
		pthread_mutex_lock(p->stop_lock);
		*p->die_index = p->num;
		print_event(p->num, get_time(p->start_time), DIE);
		pthread_mutex_unlock(p->stop_lock);
		return (1);
	}
	if (*flag && p->ctw == p->live_config->ctw)
	{
		pthread_mutex_lock(p->stop_lock);
		*p->well_fed_num += 1;
		pthread_mutex_unlock(p->stop_lock);
		*flag = 0;
	}
	pthread_mutex_unlock(p->stage_lock);
	return (0);
}
