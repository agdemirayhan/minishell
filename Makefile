# Variables
NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror 
# CFLAGS = -fsanitize=address -g
LDFLAGS = -L/opt/homebrew/opt/readline/lib -L$(LIBFT_DIR) #added these two lines
CPPFLAGS = -I/opt/homebrew/opt/readline/include -I$(LIBFT_DIR) #added these two lines bcs it wasn't working on my pc without these flags. just comment them out here and in Name target as well if it broke yours
LIBS = -lreadline -lft

# DEBUGGER. Use 'make debug' in order to run it. Change the input string in main.c
DEBUG_FLAG = 0
ifeq ($(MAKECMDGOALS),debug)
    CFLAGS += -g -DDEBUG=1
    DEBUG_FLAG = 1
endif

SRCS =	main.c cmd_parser.c builtin_commands.c exec.c signal_handling.c e_env.c e_export.c \
		e_unset.c e_pwd.c exp_env_var.c remove_quotes.c print_cmds.c redir.c heredoc_handler.c \
		pipes.c shlvl.c parsing_utils.c exec_utils.c parsing_utils2.c token_spacer.c \
		split_with_quotes.c
OBJS = $(SRCS:.c=.o)

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

# Targets
all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LDFLAGS) $(LIBS)

$(LIBFT):
	make -C $(LIBFT_DIR)

clean:
	rm -f $(OBJS)
	make -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	make -C $(LIBFT_DIR) fclean

re: fclean all

debug: $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $(NAME) $(OBJS) $(LDFLAGS) $(LIBS)
	@echo "\033[0;31mDebug mode activated. DEBUG=$(DEBUG_FLAG)\033[0m"

.PHONY: all clean fclean re