NAME		=	philo

FILES		=	death.c error_and_print.c fill_stats.c ft_atoi.c \
				life_cycle.c philo.c time.c

SRCS		=	$(FILES)
OBJS		=	$(SRCS:.c=.o)

CC			=	cc
CFLAGS		=	-Wall -Wextra -Werror

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

all: $(NAME)

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re