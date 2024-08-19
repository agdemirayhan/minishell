#include "minishell.h"

int	is_builtin(char *command)
{
	return (ft_strncmp(command, "cd", ft_strlen("cd")) == 0 || ft_strncmp(command, "exit", ft_strlen("exit")) == 0);
}

void	execute_builtin(char **args)
{
	if (ft_strncmp(args[0], "cd", ft_strlen("cd")) == 0)
	{
		if (chdir(args[1]) != 0)
		{
			perror("minishell");
		}
	}
	else if (ft_strncmp(args[0], "exit", ft_strlen("exit")) == 0)
	{
		exit(0);
	}
}
