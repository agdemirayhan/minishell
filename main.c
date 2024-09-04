#include "minishell.h"

void	init_shell(void)
{
	signal(SIGINT, handle_signals);
	signal(SIGQUIT, SIG_IGN); // Ignore quit signal
}

int	main(int argc, char **argv, char **envp)
{
	char	*input;
	//t_data	data;
	t_env	*env_list;
	//int		i = 0;

	if (argc != 1 || argv == NULL)
		exit(EXIT_FAILURE);
	//while (envp[i] != NULL)
	//{
	//	printf("%d: %s\n", i, envp[i]);
	//	i++;
	//}
	env_list = init_env_list(envp);
	init_shell(); // Call it here
	while (1)
	{
		input = readline("minishell> ");
		if (!input)
			break ;
		add_history(input);
		parse_command(input, env_list);
		free(input);
	}
	free_env_list(&env_list);
	//free_env_list(&data);
	return (0);
}
