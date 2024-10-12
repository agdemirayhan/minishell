/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aagdemir <aagdemir@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 16:52:39 by aagdemir          #+#    #+#             */
/*   Updated: 2024/10/12 16:57:09 by aagdemir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_data	*initialize_shell(char **envp)
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	if (!data)
		exit(EXIT_FAILURE);
	data->shlvl_history = NULL;
	data->env_list = NULL;
	data->mini_count = 1;
	data->prev_exit_stat = 0;
	data->env_list = init_env_list(envp);
	if (!data->env_list)
	{
		free(data);
		exit(EXIT_FAILURE);
	}
	return (data);
}

void	handle_debug_mode(t_data *data)
{
	char	*input;

	input = getenv("DEBUG_INPUT");
	if (input == NULL)
	{
		input = ft_strdup("echo HelloWorld | grep Hello");
	}
	printf("Debug mode with input: %s\n", input);
	add_history(input);
	parse_command(input, data);
	free(input);
}

void	main_loop(t_data *data)
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
		parse_command(input, data);
		free(input);
	}
	free_env_list(&(data->env_list));
	free(data);
}

int	main(int argc, char **argv, char **envp)
{
	t_data	*data;

	if (argc != 1 || argv == NULL)
		exit(EXIT_FAILURE);
	data = initialize_shell(envp);
	if (DEBUG)
		handle_debug_mode(data);
	else
		main_loop(data);
	return (0);
}
