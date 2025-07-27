NAME =		philosophers

FILES =		death.c error_and_print.c fill_stats.c ft_atoi.c \
			life_cycle.c philo.c time.c

PATH =		./philo/
SRCS =		$(addprefix $(PATH), $(FILES))

OBJS = 		$(SRCS:.c=.o)

CC =		cc
CFLAGS =	-Wall -Wextra -Werror

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)


all: $(NAME)

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(OBJS) $(NAME)

re: fclean all

.PHONY: all clean fclean re