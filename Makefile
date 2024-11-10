# Variables
NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror
# CFLAGS = -fsanitize=address -g
LDFLAGS =-L$(LIBFT_DIR) #added these two lines
# CPPFLAGS = -I/opt/homebrew/opt/readline/include -I$(LIBFT_DIR) #added these two lines bcs it wasn't working on my pc without these flags. just comment them out here and in Name target as well if it broke yours
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

# Different Shades of Magenta
MAGENTA_BRIGHT = \033[38;5;201m
MAGENTA_LIGHT = \033[38;5;206m
MAGENTA_DEEP = \033[38;5;165m
MAGENTA_PALE = \033[38;5;182m
MAGENTA_DARK = \033[38;5;91m

RED = \033[0;31m
GREEN = \033[0;32m
YELLOW = \033[38;5;229m
BLUE = \033[0;34m
MAGENTA = \033[0;35m
CYAN = \033[0;36m
WHITE = \033[0;37m
RESET = \033[0m

#MINI_LOADING_MSG = "$(MAGENTA_BRIGHT)Loading minishell...$(RESET)"
LOADING_COMPLETE = \
					"		\n"\
					"                    ▒▒▒▒▒▒▒▒▒▒▒▒▒█\n"\
					"                ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒█\n"\
					"          ██▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓$(MAGENTA_PALE)███$(RESET)▒▒$(MAGENTA_PALE)███$(RESET)█\n"\
					"        ██▓▓███▒▒▒▒▒▒▒▒▒▒▒▒▒▒$(MAGENTA_PALE)█▓▓▓██▓▓▓█$(RESET)▒▒█\n"\
					"       ██▒██▒▒▓▓▓▓▒██▒▒▒▒▒▒▒▓▒$(MAGENTA_PALE)███$(RESET)▒▒$(MAGENTA_PALE)███$(RESET)▒▒▒█\n"\
					"        ██▒▓░░░░░░▓▓▒▒▒▒▓▓▓░░░░░▓▓▓▓▓▒▒▒▒▒▒▒█\n"\
					"        █▓░░░▒▓░░░▓▒▒▒▒▒▒▓░▓▒░░░░░▓▓▓▒▒▒▒▒▒█\n"\
					"       ██▓░░░░░░▓▓▒▒▒▒▒▒▒▓░░░░░░░░▓▓▓▒▒▓▓▒▒▒█\n"\
					"       █▒▓▓░░░▓▓▒▒▒▒▒▒▒▒▒▒▓▓▓░░░░░▓▓▓▓▒▒▒▓▓▒█\n"\
					"       ███▓▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▒█▒▒▓▒▒\n"\
					"      ███▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▒██▒▒▒█\n"\
					"      ██████▒▒▒▒▒▓▓▓▓▓▓▒▒▒▒▒▒▓▓▓▓▓▓▓▒█████\n"\
					"      ████████▒▒▒▒███▒▒▒▒▒▒▒▓▓▓▓▓▓▒▒████▒██\n"\
					"        ████████▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒██▒▒▒▒██\n"\
					"            ██████▒▒▒▒▒▒█████▒▒▒████▒▒▒\n"\
					"               ███████████████████▒▒▒▒▒\n"\
					"             █▒▒▓▓▓▒▒███▒▓▒▒▒▒█████▒▒▒▒$(MAGENTA_DARK)	█▀▄▀█ $(MAGENTA_DEEP)█ $(MAGENTA_BRIGHT)█▄░█ $(MAGENTA_LIGHT)█ $(MAGENTA_PALE)█▀ $(MAGENTA_LIGHT)█░█ $(MAGENTA_BRIGHT)█▀▀ $(MAGENTA_DEEP)█░░ $(MAGENTA_DARK)█░░ $(RESET)\n"\
					"                             ▒▒▒▒▒▒▒   $(MAGENTA_DARK)	█░▀░█ $(MAGENTA_DEEP)█ $(MAGENTA_BRIGHT)█░▀█ $(MAGENTA_LIGHT)█ $(MAGENTA_PALE)▄█ $(MAGENTA_LIGHT)█▀█ $(MAGENTA_BRIGHT)██▄ $(MAGENTA_DEEP)█▄▄ $(MAGENTA_DARK)█▄▄ $(RESET)\n"\


# Targets
all: $(LIBFT) mini_message $(NAME)

mini_message:
	@printf "$(MAGENTA_BRIGHT)Loading minishell  "
	@spin='-\|/'; \
	i=0; \
	while [ $$i -lt 12 ]; do \
		printf "\b$$(expr $$i % 4)"; \
		printf "\b$${spin:$$((i % 4)):1}"; \
		sleep 0.25; \
		i=$$((i + 1)); \
	done
	@printf "$(RESET)\n"

$(NAME): $(LIBFT) $(OBJS)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LDFLAGS) $(LIBS)
	@echo $(LOADING_COMPLETE)

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
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LDFLAGS) $(LIBS)
	@echo "\033[0;31mDebug mode activated. DEBUG=$(DEBUG_FLAG)\033[0m"

.PHONY: all clean fclean re