#include "minishell.h"

void	handle_builtin(t_mini *mini_cmd, t_data *data)
{
	int	saved_stdin;
	int	saved_stdout;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	// Handle input redirection
	if (mini_cmd->infile != STDIN_FILENO)
	{
		dup2(mini_cmd->infile, STDIN_FILENO);
		close(mini_cmd->infile);
	}
	// Handle output redirection
	if (mini_cmd->outfile != STDOUT_FILENO)
	{
		dup2(mini_cmd->outfile, STDOUT_FILENO);
		close(mini_cmd->outfile);
	}
	// Execute the built-in command
	execute_builtin(mini_cmd->full_cmd, data);
	// Restore original file descriptors
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
}

void	handle_external_command(t_mini *mini_cmd)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
	{
		if (mini_cmd->infile != STDIN_FILENO)
		{
			dup2(mini_cmd->infile, STDIN_FILENO);
			close(mini_cmd->infile);
		}
		if (mini_cmd->outfile != STDOUT_FILENO)
		{
			dup2(mini_cmd->outfile, STDOUT_FILENO);
			close(mini_cmd->outfile);
		}
		execute_command(mini_cmd->full_cmd);
		exit(EXIT_FAILURE);
	}
	else if (pid < 0)
		perror("minishell");
	else
		waitpid(pid, &status, 0);
}

void	execute_mini_command(t_mini *mini_cmd, t_data *data)
{
	int		saved_stdin;
	int		saved_stdout;
	pid_t	pid;
	int		status;

	if (is_builtin(mini_cmd->full_cmd[0]))
	{
		handle_builtin(mini_cmd, data);
	}
	else
	{
		handle_external_command(mini_cmd);
	}
}
