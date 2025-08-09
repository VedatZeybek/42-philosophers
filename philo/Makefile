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

asan: $(OBJS)
	$(CC) $(CFLAGS) -fsanitize=address $(OBJS) -o $(NAME)
	./$(NAME) 4 410 200 200 10

tsan: $(OBJS)
	$(CC) $(CFLAGS) -fsanitize=thread  $(OBJS) -o $(NAME)
	./$(NAME) 4 410 200 200 10

valgrind: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	valgrind --tool=drd ./$(NAME) 4 410 200 200 5


clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re valgrind