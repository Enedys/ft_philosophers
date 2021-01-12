SRCS			:=	initialize_simulation.c  libft_utils.c  lifestyle.c  main.c  process_input.c  utils.c
OBJS			= $(SRCS:.c=.o)

CC				= gcc
RM				= rm -f
CFLAGS			= -Wall -Wextra -Werror
LIBS			= -pthread

NAME			= philo_one

all:			$(NAME)

$(NAME):		$(OBJS)
				gcc ${CFLAGS} -o ${NAME} ${OBJS} ${LIBS}

clean:
				$(RM) $(OBJS)

fclean:			clean
				$(RM) $(NAME)

re:				fclean $(NAME)

.PHONY:			all clean fclean re