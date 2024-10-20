/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aagdemir <aagdemir@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 21:46:38 by aagdemir          #+#    #+#             */
/*   Updated: 2024/10/21 00:05:51 by aagdemir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	execute_pipes(t_list *cmds, t_data *data)
{
	int		pipefd[2];
	int		in_fd = 0;
	pid_t	pid;
	t_list	*cmd_node = cmds;
	t_mini	*mini_cmd;
	int		status;
	pid_t	child_pids[1024];
	int		child_count = 0;
	int		i;

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
				if (dup2(in_fd, STDIN_FILENO) == -1)
				{
					perror("dup2 in_fd");
					exit(EXIT_FAILURE);
				}
				close(in_fd);
			}
			if (cmd_node->next != NULL)
			{
				close(pipefd[0]); // Close unused read end
				if (dup2(pipefd[1], STDOUT_FILENO) == -1)
				{
					perror("dup2 pipefd[1]");
					exit(EXIT_FAILURE);
				}
				close(pipefd[1]);
			}
			if (is_builtin(mini_cmd->full_cmd[0]))
			{
				execute_builtin(mini_cmd->full_cmd, data);
				exit(EXIT_SUCCESS);
			}
			else
			{
				char *e_path = find_exec(mini_cmd->full_cmd[0]);
				if (!e_path)
				{
					ft_putstr_fd("minishell: ", STDERR_FILENO);
					ft_putstr_fd(mini_cmd->full_cmd[0], STDERR_FILENO);
					ft_putstr_fd(": command not found\n", STDERR_FILENO);
					exit(EXIT_FAILURE);
				}
				if (execve(e_path, mini_cmd->full_cmd, NULL) == -1)
				{
					perror("execve");
					exit(EXIT_FAILURE);
				}
			}
		}
		else // Parent process
		{
			child_pids[child_count++] = pid;
			if (in_fd != 0)
				close(in_fd);
			if (cmd_node->next != NULL)
			{
				close(pipefd[1]);
				in_fd = pipefd[0];
			}
			else
			{
				if (pipefd[0] > 0)
					close(pipefd[0]);
			}
			cmd_node = cmd_node->next;
		}
	}
	i = 0;
	while (i < child_count)
	{
		waitpid(child_pids[i], &status, 0);
		i++;
	}
}