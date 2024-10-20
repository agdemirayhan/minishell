/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aagdemir <aagdemir@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 14:10:37 by aagdemir          #+#    #+#             */
/*   Updated: 2024/10/20 21:34:44 by aagdemir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	builtin(t_data *data, t_mini *mini_cmd)
{
	int	saved_stdin;
	int	saved_stdout;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
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
	execute_builtin(mini_cmd->full_cmd, data);
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
}

void	othercommands_helper(t_data *data, t_mini *mini_cmd, DIR *dir)
{
	char	*e_path;

	e_path = find_exec(mini_cmd->full_cmd[0]);
	if (!e_path)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(mini_cmd->full_cmd[0], STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		data->prev_exit_stat = 127;
		return ;
	}
	if (access(e_path, X_OK) == -1 || dir)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(mini_cmd->full_cmd[0], STDERR_FILENO);
		ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
		data->prev_exit_stat = 126;
		free(e_path);
		return ;
	}
	free(e_path);
}

void	forker(t_data *data, t_mini *mini_cmd)
{
	if (mini_cmd->infile != STDIN_FILENO)
	{
		if (dup2(mini_cmd->infile, STDIN_FILENO) == -1)
		{
			perror("minishell: dup2 failed for infile");
			exit(1);
		}
		close(mini_cmd->infile);
	}
	if (mini_cmd->outfile != STDOUT_FILENO)
	{
		if (dup2(mini_cmd->outfile, STDOUT_FILENO) == -1)
		{
			perror("minishell: dup2 failed for outfile");
			exit(1);
		}
		close(mini_cmd->outfile);
	}
	execute_command(mini_cmd->full_cmd, data);
	exit(data->prev_exit_stat);
}

void	othercommands(t_data *data, t_mini *mini_cmd)
{
	pid_t	pid;
	int		status;
	DIR		*dir;

	if (mini_cmd && mini_cmd->full_cmd)
		dir = opendir(mini_cmd->full_cmd[0]);
	othercommands_helper(data, mini_cmd, dir);
	pid = fork();
	if (pid == 0)
	{
		forker(data, mini_cmd);
	}
	else if (pid < 0)
	{
		perror("minishell: fork failed");
		data->prev_exit_stat = 1;
	}
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			data->prev_exit_stat = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			data->prev_exit_stat = 128 + WTERMSIG(status);
	}
}

void	parse_command(char **args, t_data *data, t_prompt *test)
{
	t_list	*cmd_node;
	t_mini	*mini_cmd;

	test->cmds = fill_nodes(args, data);
	if (test->cmds && test->cmds->next != NULL)
		execute_pipes(test->cmds, data);
	else
	{
		cmd_node = test->cmds;
		while (cmd_node)
		{
			mini_cmd = (t_mini *)cmd_node->content;
			if (mini_cmd && mini_cmd->full_cmd && mini_cmd->full_cmd[0])
			{
				if (is_builtin(mini_cmd->full_cmd[0]))
					builtin(data, mini_cmd);
				else
					othercommands(data, mini_cmd);
				check_and_update_shlvl(data, mini_cmd);
			}
			cmd_node = cmd_node->next;
		}
	}
	ft_lstclear(&test->cmds, free_mini);
}
