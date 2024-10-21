/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aagdemir <aagdemir@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 16:41:50 by aagdemir          #+#    #+#             */
/*   Updated: 2024/10/21 16:45:03 by aagdemir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	child_process_helper(t_pipe_e_st *e_st, t_list *cmd_node,
		t_mini *mini_cmd, t_data *data)
{
	if (e_st->in_fd != 0)
	{
		if (dup2(e_st->in_fd, STDIN_FILENO) == -1)
		{
			perror("dup2 in_fd");
			exit(EXIT_FAILURE);
		}
		close(e_st->in_fd);
	}
	if (cmd_node->next != NULL)
	{
		close(e_st->pipefd[0]);
		if (dup2(e_st->pipefd[1], STDOUT_FILENO) == -1)
		{
			perror("dup2 pipefd[1]");
			exit(EXIT_FAILURE);
		}
		close(e_st->pipefd[1]);
	}
	if (is_builtin(mini_cmd->full_cmd[0]))
	{
		execute_builtin(mini_cmd->full_cmd, data);
		exit(EXIT_SUCCESS);
	}
}

void	setup_pipe(t_pipe_e_st *e_st)
{
	if (pipe(e_st->pipefd) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
}

void	wait_for_all_children(t_pipe_e_st *e_st)
{
	int	i;

	i = 0;
	while (i < e_st->child_count)
	{
		waitpid(e_st->child_pids[i], &e_st->status, 0);
		i++;
	}
}
