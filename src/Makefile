# Variables
NAME = minishell
CC = cc
# CFLAGS = -Wall -Wextra -Werror -fsanitize=address
# CFLAGS = -fsanitize=address -g
CFLAGS =  -g
LDFLAGS = -L/opt/homebrew/opt/readline/lib -L$(LIBFT_DIR) #added these two lines
CPPFLAGS = -I/opt/homebrew/opt/readline/include -I$(LIBFT_DIR) #added these two lines bcs it wasn't working on my pc without these flags. just comment them out here and in Name target as well if it broke yours
LIBS = -lreadline -lft

# DEBUGGER. Use 'make debug' in order to run it. Change the input string in main.c
DEBUG_FLAG = 0
ifeq ($(MAKECMDGOALS),debug)
    CFLAGS += -g -DDEBUG=1
    DEBUG_FLAG = 1
endif

SRCS =	main.c \
		env/e_env.c \
		env/exp_env_var.c \
		execution/builtin_commands.c \
		execution/exec.c \
		execution/heredoc_handler.c \
		execution/pipes.c \
		execution/pipes2.c \
		execution/redir.c \
		execution/redir2.c \
		execution/shlvl.c \
		execution/signal_handling.c \
		parsing/cmd_parser.c \
		parsing/fill_nodes.c \
		parsing/parsing_commands.c \
		parsing/remove_quotes.c \
		parsing/splitter.c \
		parsing/token_spacer.c \
		parsing/tokenizer.c \
		builtins/b_exit.c \
		builtins/b_echo.c \
		builtins/b_pwd.c \
		builtins/b_cd.c \
		utils/e_export.c \
		utils/e_pwd.c \
		utils/e_unset.c \
		utils/print_cmds.c \
		utils/free_strarr.c
OBJS = $(SRCS:.c=.o)

LIBFT_DIR = ../libft
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