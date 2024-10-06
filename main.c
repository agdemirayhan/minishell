#include "minishell.h"

void	init_shell(void)
{
	signal(SIGINT, handle_signals);
	signal(SIGQUIT, SIG_IGN); // Ignore quit signal
}

int	main(int argc, char **argv, char **envp)
{
	char	*input;
	t_data	*data;

	// t_env	*env_list;
	// int		i = 0;
	if (argc != 1 || argv == NULL)
		exit(EXIT_FAILURE);
	// while (envp[i] != NULL)
	//{
	//	printf("%d: %s\n", i, envp[i]);
	//	i++;
	//}
	data = malloc(sizeof(t_data));
	if (!data)
	{
		exit(EXIT_FAILURE);
	}
	data->env_list = init_env_list(envp);
	if (!data->env_list)
	{
		free(data);
		exit(EXIT_FAILURE);
	}
	init_shell(); // Call it here
	if (DEBUG)
	{
		input = getenv("DEBUG_INPUT");
		if (input == NULL)
		{
			input = "cat << test | grep te";
		}
		printf("Debug mode with input: %s\n", input);
		add_history(input);
		parse_command(input, data);
	}
	else
	{
		while (1)
		{
			input = readline("minishell> ");
			if (!input)
				break ;
			add_history(input);
			parse_command(input, data);
			free(input);
		}
		free_env_list(&(data->env_list));
		free(data);
	}
	// free_env_list(&data);
	return (0);
}
