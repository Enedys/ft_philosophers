#include "includes.h"

int					return_error(char *strerror)
{
	g_strerror = strerror ? strerror : CORRECT_OUTPUT;
	if (strerror)
		write(STDOUT_FILENO, g_strerror, ft_strlen(g_strerror));
	return (strerror ? 1 : 0);
}

static int			is_number(char *s)
{
	int		i;

	i = 0;
	while (s[i] >= '0' && s[i] <= '9')
		i++;
	if (!s[i])
		return (1);
	return (0);
}

static int			infill_input_struct(t_input_data *data, char **argv)
{
	int		err;

	err = 0;
	if (!is_number(argv[1]) || !((data->amount = ft_atoi(argv[1])) > 1))
		err = 1;
	if (!is_number(argv[2]) || !((data->ttd = ft_atoi(argv[2])) > 10))
		err = 1;
	if (!is_number(argv[3]) || !((data->tte = ft_atoi(argv[3])) > 10))
		err = 1;
	if (!is_number(argv[4]) || !((data->tts = ft_atoi(argv[4])) > 10))
		err = 1;
	if (argv[5] && \
		(!is_number(argv[5]) || !((data->ctw = ft_atoi(argv[5])) > 0)))
		err = 1;
	else if (!argv[5])
		data->ctw = -1;
	return (err);
}

int					load_input_data(t_input_data *input_data,\
										int argc, char **argv)
{
	int				res;

	if (argc > 6 || argc < 5)
		return (return_error(INVALID_ARGS));
	memset(input_data, 0, sizeof(t_input_data));
	if ((res = infill_input_struct(input_data, argv)))
		return (return_error(BAD_ARG));
	if (input_data->ttd >= input_data->tte && \
		input_data->ttd >= input_data->tts)
		input_data->min_gap = input_data->ttd;
	else if (input_data->tte >= input_data->ttd && \
			input_data->tte >= input_data->tts)
		input_data->min_gap = input_data->tte;
	else
		input_data->min_gap = input_data->tts;
	return (return_error(NULL));
}
