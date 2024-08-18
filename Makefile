# Variables
NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror

SRCS = main.c cmd_parser.c builtin_commands.c exec.c signal_handling.c
OBJS = $(SRCS:.c=.o)

# Targets
all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) -lreadline

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
