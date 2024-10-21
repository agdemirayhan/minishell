/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aagdemir <aagdemir@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 20:40:42 by aagdemir          #+#    #+#             */
/*   Updated: 2024/10/21 20:41:02 by aagdemir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cleanup(t_data *data)
{
	if (data)
	{
		free_env_list(&(data->env_list));
		free(data);
	}
	clear_history();
}

int	handle_syntax_error(char **args)
{
	if (args[0][0] == '|' && args[1] == NULL)
	{
		ft_putstr_fd("syntax error near unexpected token '|'\n", 2);
		free(args);
		return (-1);
	}
	return (0);
}

void	handle_debug_mode(t_data *data)
{
	char	*input;

	input = getenv("DEBUG_INPUT");
	if (input == NULL)
		input = "asdas";
	add_history(input);
	parsing_handler(input, data);
}

void	interactive_shell_loop(t_data *data)
{
	char	*input;

	while (1)
	{
		signal(SIGINT, handle_signals);
		signal(SIGQUIT, SIG_IGN);
		input = readline("minishell> ");
		if (!input)
		{
			printf("exit\n");
			break ;
		}
		if (input[0] != '\0')
			add_history(input);
		parsing_handler(input, data);
		free(input);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_data	*data;

	if (argc != 1 || argv == NULL)
		exit(EXIT_FAILURE);
	data = initialize_data(envp);
	init_shell(data);
	if (DEBUG)
		handle_debug_mode(data);
	else
		interactive_shell_loop(data);
	free_env_list(&(data->env_list));
	cleanup(data);
	return (0);
}
