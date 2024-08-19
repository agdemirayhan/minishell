#include "minishell.h"

void	execute_command(char **args)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
	{ // Child process
		if (execvp(args[0], args) == -1) //execvp not allowed
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
