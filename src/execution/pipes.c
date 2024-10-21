/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aagdemir <aagdemir@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 21:46:38 by aagdemir          #+#    #+#             */
/*   Updated: 2024/10/21 16:44:41 by aagdemir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	execute_pipes(t_list *cmds, t_data *data);
void	execute_pipe_command(t_list *cmd_node, t_data *data, t_pipe_e_st *e_st);
void	setup_pipe(t_pipe_e_st *e_st);
void	handle_child_process(t_mini *mini_cmd, t_list *cmd_node, t_data *data,
			t_pipe_e_st *e_st);
void	parentprocess(t_pipe_e_st *e_st, t_list *cmd_node);
void	wait_for_all_children(t_pipe_e_st *e_st);
void	child_process_helper(t_pipe_e_st *e_st, t_list *cmd_node,
			t_mini *mini_cmd, t_data *data);

void	execute_pipes(t_list *cmds, t_data *data)
{
	t_list		*cmd_node;
	t_pipe_e_st	e_st;

	cmd_node = cmds;
	e_st.in_fd = 0;
	e_st.child_count = 0;
	while (cmd_node)
	{
		execute_pipe_command(cmd_node, data, &e_st);
		cmd_node = cmd_node->next;
	}
	wait_for_all_children(&e_st);
}

void	execute_pipe_command(t_list *cmd_node, t_data *data, t_pipe_e_st *e_st)
{
	t_mini	*mini_cmd;

	mini_cmd = (t_mini *)cmd_node->content;
	if (cmd_node->next != NULL)
		setup_pipe(e_st);
	e_st->pid = fork();
	if (e_st->pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (e_st->pid == 0)
	{
		handle_child_process(mini_cmd, cmd_node, data, e_st);
	}
	else
	{
		parentprocess(e_st, cmd_node);
	}
}

void	handle_child_process(t_mini *mini_cmd, t_list *cmd_node, t_data *data,
		t_pipe_e_st *e_st)
{
	char	*e_path;

	if (e_st->in_fd != 0 || cmd_node->next != NULL
		|| is_builtin(mini_cmd->full_cmd[0]))
	{
		child_process_helper(e_st, cmd_node, mini_cmd, data);
	}
	e_path = find_exec(mini_cmd->full_cmd[0]);
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

void	parentprocess(t_pipe_e_st *e_st, t_list *cmd_node)
{
	e_st->child_pids[e_st->child_count++] = e_st->pid;
	if (e_st->in_fd != 0)
		close(e_st->in_fd);
	if (cmd_node->next != NULL)
	{
		close(e_st->pipefd[1]);
		e_st->in_fd = e_st->pipefd[0];
	}
	else
	{
		close(e_st->pipefd[0]);
	}
}
