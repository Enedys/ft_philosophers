#include <unistd.h>

static int	ft_dim(int n)
{
	int	iter;

	iter = 0;
	while ((n /= 10) != 0)
		iter++;
	return (iter + 1);
}

size_t		ft_strlen(const char *s)
{
	const char	*str;

	str = s;
	while (*str)
		str++;
	return (str - s);
}

char		*ft_itoa_buf(int n, char *p)
{
	int		flag;
	int		iter;
	int		shift;

	iter = ft_dim(n) + 1;
	flag = 1;
	if (n < 0 && iter++)
		flag = -1;
	shift = iter - 1;
	iter -= 2;
	*(p + iter) = (n % 10) * flag + 48;
	while (iter-- && (n /= 10) != 0)
		*(p + iter) = (n % 10) * flag + 48;
	if (flag == -1)
		*p = '-';
	return (p + shift);
}

size_t		ft_strlcpy(char *d, const char *src, size_t n)
{
	const char	*s;

	s = src;
	if (n != 0)
	{
		while (--n && *s)
			*d++ = *s++;
		*d = '\0';
	}
	while (*s)
		s++;
	return (s - src);
}

int			ft_atoi(char *str)
{
	int		sign;
	long	resu;

	resu = 0;
	sign = 1;
	while (*str == ' ' || (*str >= '\t' && *str <= '\r'))
		str++;
	if ((*str == '+' || *str == '-'))
		sign = (*str++) == '-' ? -1 : 1;
	while (*str == '0')
		str++;
	while (*str >= '0' && *str <= '9')
	{
		resu = resu * 10 + sign * ((*str++) - '0');
		if ((resu > 0) != (sign > 0))
			return (-(1 + sign) / 2);
	}
	return ((int)resu);
}
