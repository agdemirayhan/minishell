/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aagdemir <aagdemir@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 14:16:08 by msoklova          #+#    #+#             */
/*   Updated: 2024/08/18 17:18:50 by aagdemir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

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

/* Function prototypes */
void	init_shell(void);
void	handle_signals(int signo);
void	parse_command(char *input);
void	execute_command(char **args);
int		is_builtin(char *command);
void	execute_builtin(char **args);

#endif
