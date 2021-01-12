#include "includes.h"

int			print_time(t_philsopher *p, char *s)
{
	int		res;
	int		time;

	res = 0;
	if ((res = sem_wait(p->write_lock)))
		return (1);
	if ((time = get_time(p->start_time)) == -1)
		return (1);
	if (print_event(p->num, time, s) == -1)
		res |= 1;
	res |= sem_post(p->write_lock);
	return (res);
}

int			print_event(int num, int time, char *s)
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

int			clean_data(t_philos_list *phs)
{
	clean_philosophers(phs->philosophers, phs->amount);
	return (1);
}

int			get_time(struct timeval *start_time)
{
	struct timeval	current_time;

	if (gettimeofday(&current_time, NULL))
		return (-1);
	return ((current_time.tv_sec - start_time->tv_sec) * 1000 +\
			(current_time.tv_usec - start_time->tv_usec) / 1000);
}
