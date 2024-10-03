#include "minishell.h"

void execute_pipes(t_list *cmds)
{
	int		pipefd[2];
	int		in_fd = 0;
	pid_t	pid;
	int		status;
	t_list	*cmd_node = cmds;
	t_mini	*mini_cmd;

	while (cmd_node)
	{
		mini_cmd = (t_mini *)cmd_node->content;
		if (cmd_node->next != NULL)
		{
			if (pipe(pipefd) == -1)
			{
				perror("pipe");
				exit(EXIT_FAILURE);
			}
		}
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		if (pid == 0) // Child process
		{
			if (in_fd != 0)
			{
				dup2(in_fd, STDIN_FILENO);
				close(in_fd);
			}
			if (cmd_node->next != NULL)
			{
				dup2(pipefd[1], STDOUT_FILENO);
				close(pipefd[0]);
			}
			execute_command(mini_cmd->full_cmd);
			perror("execve");
			exit(EXIT_FAILURE);
		}
		else // Parent process
		{
			if (cmd_node->next != NULL)
			{
				close(pipefd[1]);
			}
			in_fd = pipefd[0];
			cmd_node = cmd_node->next;
		}
	}
}

