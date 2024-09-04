#include "minishell.h"

void	execute_command(char **args)
{
	pid_t	pid;
	int		status;

	char	*path_env;
	path_env = getenv("PATH");
	// printf("%s\n",path_env);
	if (path_env == NULL)
	{
		return ;
	}
	pid = fork();
	if (pid == 0)
	{										// Child process
		if (execve(path_env,args, NULL) == -1)
			// It breaks with pwd command but this is the way
		{
			perror("minishell");
		}
		exit(EXIT_FAILURE);
	}
	else if (pid < 0)
	{ // Error forking
		perror("minishell");
	}
	else
	{ // Parent process
		do
		{
			waitpid(pid, &status, WUNTRACED);
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}
}
