BONUS_NAME	=	philo_bonus

BONUS_FILES	=	error_bonus.c fill_stats_bonus.c ft_atoi_bonus.c \
				philo_bonus.c philo_process_bonus.c time_bonus.c \
				monitor_bonus.c

BONUS_SRCS	=	$(BONUS_FILES)
BONUS_OBJS	=	$(BONUS_SRCS:.c=.o)

CC			=	cc
CFLAGS		=	-Wall -Wextra -Werror

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BONUS_NAME): $(BONUS_OBJS)
	$(CC) $(CFLAGS) $(BONUS_OBJS) -o $(BONUS_NAME)

all: $(BONUS_NAME)

clean:
	rm -rf $(BONUS_OBJS)

fclean: clean
	rm -rf $(BONUS_NAME)

re: fclean all

.PHONY: all clean fclean re