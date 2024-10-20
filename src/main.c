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

int	process_input(char *input, t_data *data, char ***args)
{
	char	*new_str;
	char	*expanded_str;
	char	*trimmed_expanded_str;

	new_str = token_spacer(input);
	if (!new_str)
		return (-1);
	expanded_str = expand_env_vars(new_str, data);
	free(new_str);
	trimmed_expanded_str = ft_strtrim(expanded_str, " \t\n");
	free(expanded_str);
	*args = split_with_quotes(trimmed_expanded_str, " ");
	free(trimmed_expanded_str);
	if (!(*args) || (*args)[0] == NULL)
	{
		free(*args);
		return (-1);
	}
	return (0);
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

void	trim_arguments(char **args)
{
	int		i;
	char	*trimmed_arg;

	i = 0;
	while (args[i])
	{
		trimmed_arg = ft_strtrim_all(args[i]);
		free(args[i]);
		args[i] = trimmed_arg;
		i++;
	}
}

int	parsing_helpers(char *input, t_data *data, char ***args)
{
	int	ret;

	ret = process_input(input, data, args);
	if (ret == -1)
		return (-1);
	ret = handle_syntax_error(*args);
	if (ret == -1)
		return (-1);
	trim_arguments(*args);
	return (0);
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
}

t_data	*initialize_data(char **envp)
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
		cleanup(data);
		exit(EXIT_FAILURE);
	}
	return (data);
}

void	handle_debug_mode(t_data *data)
{
	char	*input;

	input = getenv("DEBUG_INPUT");
	if (input == NULL)
		input = "echo test";
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
