NAME = libft.a

# Colors
RED = \033[0;31m
GREEN = \033[0;32m
YELLOW = \033[38;5;229m
BLUE = \033[0;34m
MAGENTA = \033[0;35m
LIGHT_CYAN = \033[1;36m
CYAN = \033[0;36m
DARK_CYAN = \033[2;36m
WHITE = \033[0;37m
RESET = \033[0m

SRCS =  ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c ft_isprint.c\
		ft_strlen.c ft_memset.c ft_bzero.c ft_memcpy.c ft_memmove.c ft_strlcpy.c\
		ft_strlcat.c ft_toupper.c ft_tolower.c ft_strchr.c\
		ft_strrchr.c  ft_memchr.c ft_memcmp.c ft_strnstr.c ft_atoi.c\
 		ft_calloc.c  ft_substr.c ft_strdup.c ft_itoa.c ft_putchar_fd.c\
		ft_putstr_fd.c ft_putnbr_fd.c ft_putendl_fd.c ft_strjoin.c\
		ft_strtrim.c ft_strncmp.c ft_split.c ft_strmapi.c ft_striteri.c\
		ft_lst_clear.c ft_lst_size.c ft_lst_traverse.c ft_lst_insertattail.c \
		ft_lst_newlist.c ft_strcmp.c ft_strndup.c

OBJS = $(SRCS:.c=.o)

CC = cc

CFLAGS = -Wall -Wextra -Werror
#LOADING_MSG = "$(CYAN)Loading libft...$(RESET)"
LOADING_COMPLETE = \
					"		\n"\
					"$(DARK_CYAN)	█░░░$(CYAN)▀█▀░$(LIGHT_CYAN)█▀▄░$(CYAN)█▀▀░$(DARK_CYAN)▀█▀$(RESET)\n"\
					"$(DARK_CYAN)	█░░░$(CYAN)░█░░$(LIGHT_CYAN)█▀▄░$(CYAN)█▀▀░$(DARK_CYAN)░█░$(RESET)\n"\
					"$(DARK_CYAN)	▀▀▀░$(CYAN)▀▀▀░$(LIGHT_CYAN)▀▀░░$(CYAN)▀░░░$(DARK_CYAN)░▀░$(RESET)\n"\


all: message $(NAME)

message:
	@printf "$(CYAN)Loading libft  "
	@spin='-\|/'; \
	i=0; \
	while [ $$i -lt 12 ]; do \
		printf "\b$$(expr $$i % 4)"; \
		printf "\b$${spin:$$((i % 4)):1}"; \
		sleep 0.25; \
		i=$$((i + 1)); \
	done
	@printf "$(RESET)\n"

$(NAME): $(OBJS)
	@ar rcs $(NAME) $(OBJS)
	@echo $(LOADING_COMPLETE)

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJS)

fclean: clean
	@rm -f $(NAME)

re: fclean $(NAME)


.PHONY : all clean fclean re bonus
