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

SRCS =	src/main.c \
		src/env/e_env.c \
		src/env/env_helpers.c \
		src/env/exp_env_var.c \
		src/env/exp_quote_utils.c \
		src/env/exp_result_utils.c \
		src/execution/builtin_commands.c \
		src/execution/exec.c \
		src/execution/heredoc_handler.c \
		src/execution/pipes.c \
		src/execution/pipes2.c \
		src/execution/redir.c \
		src/execution/redir2.c \
		src/execution/shlvl.c \
		src/execution/signal_handling.c \
		src/parsing/cmd_parser.c \
		src/parsing/fill_nodes.c \
		src/parsing/parsing_commands.c \
		src/parsing/remove_quotes.c \
		src/parsing/splitter.c \
		src/parsing/token_spacer.c \
		src/parsing/tokenizer.c \
		src/builtins/b_exit.c \
		src/builtins/b_echo.c \
		src/builtins/b_pwd.c \
		src/builtins/b_cd.c \
		src/utils/e_export.c \
		src/utils/e_pwd.c \
		src/utils/env_helpers.c \
		src/utils/e_unset.c \
		src/utils/initialize_data.c \
		src/utils/parsing_utils.c \
		src/utils/print_cmds.c \
		src/utils/free_strarr.c
OBJS = $(SRCS:.c=.o)

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

MINI_LOADING_MSG = "Loading minishell...\n"\
"		\n"\
"	█▀▄▀█ █ █▄░█ █ █▀ █░█ █▀▀ █░░ █░░ \n"\
"	█░▀░█ █ █░▀█ █ ▄█ █▀█ ██▄ █▄▄ █▄▄ \n"\
"									\n"\
"                  ░▒▒▒▒▒▒▒▒▒▒▒▒▒░\n"\
"                ░▒▒▒▒▒▓▓▓▓▓▓▒▒▒▒▒▒▒▒▒░\n"\
"             ░▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▒▓▓▓▒▓▒░\n"\
"         ░░▒▒░░░▒▒▒▒▒▒▒▒▒▒▒░░░▒▒▒▒▓▓▓▓▓▓▒▒░\n"\
"       ░░▒░░▒▒▓▓▓▒▒░░▒▒▒▒▒▒▒▓▓▓▒▒▓▓▒░▒▓▓▒▒▒░\n"\
"        ░░▒▓██████▒░▒▒▒▒░▒▓█████▓▓▓▓▓▓▓▓▓▒▒▒░\n"\
"        ░▓███▒▓███▓▒▒▒▒▒▒▓█▓░█████▓▓▓█▓▓▓▓▒▒░\n"\
"       ░░▓██████▓▓▒▒▒▒▒▒▒▓████████▓▓▓▒▒▓▓▓▓▒░\n"\
"       ░▒▓▓███▓▓▒▒▒▒▒▒▒▒▒▒▓▓▓█████▓▓▓▓▒▒▒▓▓▒░\n"\
"       ░░░▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▒▒▓▒▓▓▓▓▓▓▓▓▒░▒▒▓▒▒\n"\
"      ░░░▒▒▒▒▒▒▒▓▒▓▓▓▓▓▓▓▓▓▒▒▓▓▓▓▓▓▓▓▒░░▒▒▒░\n"\
"      ░░░░░░░░░▒▒▒▒▒▓▓▓▓▒▒▒▒▒▒▓▓▓▓▓▓▓▒▒░░░░\n"\
"      ░░░░░░░░░░░░░▒░░▒▓▒░▒▒▒▒▓▓▓▓▓▓▒▒░░░░▒░░\n"\
"        ░░░░░░░░░░░▒▒▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒░░▒▒▒▒░░\n"\
"            ░░░░░░░▒▒░░░░░░░▒▒▒░░░░▒▒▒\n"\
"                ░░░░░░░░░░░░░░░░░░░▒▒▒▒▒\n"\
"            ░▒▒▓▓▓▒▒░░░▒▓▒▒▒▒░░░░░▒▒▒▒\n"\
"                            ▒▒▒▒▒▒▒"


# Targets
all: $(LIBFT) mini_message $(NAME)

mini_message:
	@echo $(MINI_LOADING_MSG)

$(NAME): $(LIBFT) $(OBJS)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LDFLAGS) $(LIBS)

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
	@make -C $(LIBFT_DIR)

clean:
	@rm -f $(OBJS)
	@make -C $(LIBFT_DIR) clean

fclean: clean
	@rm -f $(NAME)
	@make -C $(LIBFT_DIR) fclean

re: fclean all

debug: $(OBJS) $(LIBFT)
	@echo -e $(LOADING_MSG)
	@$(CC) $(CFLAGS) $(CPPFLAGS) -o $(NAME) $(OBJS) $(LDFLAGS) $(LIBS)
	@echo "\033[0;31mDebug mode activated. DEBUG=$(DEBUG_FLAG)\033[0m"

.PHONY: all clean fclean re