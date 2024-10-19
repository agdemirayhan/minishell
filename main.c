#include "minishell.h"

void	init_shell(t_data *data)
{
	signal(SIGINT, handle_signals);
	signal(SIGQUIT, SIG_IGN);
	data->mini_count = 1;
}

void	cleanup(t_data *data)
{
	if (data)
	{
		free_env_list(&(data->env_list)); // Free environment list
		free(data);                       // Free t_data struct itself
	}
	clear_history(); // Free the history list (from readline)
}

int	parsing_helpers(char *input, t_data *data, char ***args)
{
	char	*new_str;
	char	*expanded_str;
	int		i;
	t_list	*cmd_node;
	t_mini	*mini_cmd;
	char	*trimmed_arg;
	int		saved_stdin;
	int		saved_stdout;
	pid_t	pid;
	int		status;
	char	*trimmed_expanded_str;
	int		j;

	new_str = token_spacer(input);
	if (!new_str)
		return (-1);
	expanded_str = expand_env_vars(new_str, data);
	free(new_str);
	trimmed_expanded_str = ft_strtrim(expanded_str, " \t\n");
	free(expanded_str);
	expanded_str = trimmed_expanded_str;
	(*args) = split_with_quotes(expanded_str, " ");
	free(expanded_str);
	if (!(*args) || (*args)[0] == NULL)
	{
		free((*args));
		return (-1);
	}
	if ((*args)[0][0] == '|' && (*args)[1] == NULL)
	{
		ft_putstr_fd("syntax error near unexpected token '|'\n", 2);
		free((*args));
		return (-1);
	}
	i = 0;
	while ((*args)[i])
	{
		trimmed_arg = ft_strtrim_all((*args)[i]);
		free((*args)[i]);
		(*args)[i] = trimmed_arg;
		i++;
	}
}

void	parsing_handler(char *input, t_data *data)
{
	t_prompt	test;
	int			i;
	char		**args;

	test.cmds = NULL;
	if (parsing_helpers(input, data, &args) == -1)
		return ;
	parse_command(args, data, &test);
	// print_cmds(test.cmds);
	// i = ft_lstsize(test.cmds);
	// printf("lstsize:%d\n", i);
	// while (i-- > 0)
	// {
	// 	// waitpid(-1, &data->prev_exit_stat, 0);
	// 	// printf("data->prev_exit_stat:%d\n", data->prev_exit_stat);
	// }
}

int	main(int argc, char **argv, char **envp)
{
	char *input;
	t_data *data;

	if (argc != 1 || argv == NULL)
		exit(EXIT_FAILURE);
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
		cleanup(data);
		exit(EXIT_FAILURE);
	}
	init_shell(data);
	if (DEBUG)
	{
		input = getenv("DEBUG_INPUT");
		if (input == NULL)
		{
			input = "echo test";
		}
		// printf("Debug mode with input: %s\n", input);
		add_history(input);
		parsing_handler(input, data);
		// parse_command(input, data);
	}
	else
	{
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
			// parse_command(input, data);
			free(input);
		}
		free_env_list(&(data->env_list));
		// free(data);
	}
	cleanup(data);
	return (0);
}