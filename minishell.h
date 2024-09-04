/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aagdemir <aagdemir@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 14:16:08 by msoklova          #+#    #+#             */
/*   Updated: 2024/09/04 17:32:49 by aagdemir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <errno.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>
# include <unistd.h>

/* Readline Functions */
# include <readline/history.h>
# include <readline/readline.h>

enum		QuoteState
{
	NO_QUOTE,
	SINGLE_QUOTE,
	DOUBLE_QUOTE
};

// Enum for count tracking
enum		Counts
{
	COUNT_INDEX,
	COUNT_WORDS
};

/* token type  */
typedef enum e_ttype
{
	NOTOKEN,
	WORD,
	PIPE,
	REDIRECTION
}					t_ttype;

/* save tokens somewhere */
typedef struct s_token
{
	char			*value;
	t_ttype			ttype;
	struct s_token	*next;
}					t_token;

typedef struct s_env
{
	char			*name;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_data
{
	int		fd_copy;
	t_env	*env_list;
}	t_data;

/* token functions */
t_ttype				check_type(char *value);

/* env functions */
char	*ft_strndup(const char *s1, size_t n);
t_env	*init_env_list(char **envp);
void	print_env_list(t_env *env);
void	free_env_list(t_env **env);
//void				add_env(t_data *data, char *env_var);
//void				ft_store_env(t_data *data, char **env);
//void				initialize(t_data *data, char **env);
//void				free_env_list(t_data *data);
//void				print_env(t_data *data);

/* signals */
//void	set_signal_fn(void);
//void	set_sig(int sig);

/* Function prototypes */
void				init_shell(void);
void				handle_signals(int signo);
void				parse_command(char *input, t_env *env_list);
void				execute_command(char **args);
int					is_builtin(char *command);
void				execute_builtin(char **args);
char	**ft_split_2(char const *s, char c);

#endif
