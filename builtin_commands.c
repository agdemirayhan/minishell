#include "minishell.h"

int	is_builtin(char *command)
{
	return (ft_strncmp(command, "cd", ft_strlen("cd")) == 0
		|| ft_strncmp(command, "exit", ft_strlen("exit")) == 0
		|| ft_strncmp(command, "pwd", ft_strlen("pwd")) == 0
		|| ft_strncmp(command, "echo", ft_strlen("echo")) == 0);
}

void	execute_builtin(char **args)
{
	char	cwd[1024];
	int		i;

	// what happens when someone tries it with a directory which requires more than 1024 chars????? fails. using malloc for this?? eww.
	if (ft_strncmp(args[0], "cd", ft_strlen("cd")) == 0)
	{
		if (chdir(args[1]) != 0)
			perror("minishell");
	}
	else if (ft_strncmp(args[0], "exit", ft_strlen("exit")) == 0)
	{
		exit(0);
	}
	// with getenv pwd output was always the same. therefore it was changed with getcwd
	else if (ft_strncmp(args[0], "pwd", ft_strlen("pwd")) == 0)
	{
		if (getcwd(cwd, sizeof(cwd)) != NULL)
			printf("%s\n", cwd);
		else
			perror("minishell");
	}
	else if (ft_strncmp(args[0], "echo", ft_strlen("echo")) == 0)
	{
		i = 1;
		while (args[i])
		{
			// this needs to be updated when some global variables or struct is implemented. because it also prints extra space at the end. I don't want to count the args first then print them. it also needs to print until the next seperator. this also doesn't work now.
			printf("%s", args[i]);
			printf(" ");
			i++;
		}
		printf("\n");
	}
	/*
	OTHER CASES COME HERE ( ͡° ͜ʖ ͡° )
	*/
}
