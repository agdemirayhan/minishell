#include "minishell.h"

// MORE TESTS



void	free_mini(void *content)
{
	t_mini	*mini_cmd;

	mini_cmd = (t_mini *)content;
	if (mini_cmd)
	{
		if (mini_cmd->full_cmd)
		{
			// Free each command string in the full_cmd array
			ft_free_matrix(&(mini_cmd->full_cmd));
		}
		// Free other dynamically allocated members of mini_cmd here, if any
		free(mini_cmd); // Finally, free the t_mini structure itself
	}
}

void	free_content(void *content)
{
	t_mini	*node;

	node = content;
	ft_free_matrix(&node->full_cmd);
	free(node->full_path);
	if (node->infile != STDIN_FILENO)
		close(node->infile);
	if (node->outfile != STDOUT_FILENO)
		close(node->outfile);
	free(node);
}

char	**ft_extend_matrix(char **matrix, char *new_entry)
{
	int		i;
	char	**new_matrix;

	i = 0;
	while (matrix && matrix[i]) // Count existing elements
		i++;
	// Allocate memory for new matrix (existing elements + 1 new entry + NULL)
	new_matrix = malloc((i + 2) * sizeof(char *));
	if (!new_matrix)
		return (NULL);
	i = 0;
	while (matrix && matrix[i])
	{
		new_matrix[i] = matrix[i]; // Copy old entries
		i++;
	}
	new_matrix[i] = ft_strdup(new_entry); // Add new entry
	new_matrix[i + 1] = NULL;             // Null-terminate the array
	free(matrix);
	return (new_matrix);
}

int	is_redir(char *arg)
{
	if (!arg)
		return (0);                  // Null argument is not a redirection
	if (ft_strcmp(arg, ">") == 0 ||  // Output redirection
		ft_strcmp(arg, ">>") == 0 || // Append output redirection
		ft_strcmp(arg, "<") == 0 ||  // Input redirection
		ft_strcmp(arg, "<<") == 0)   // Here-document redirection
	{
		return (1); // It is a redirection operator
	}
	return (0); // Not a redirection operator
}

void	ft_free_matrix(char ***m)
{
	int	i;

	i = 0;
	while (m && m[0] && m[0][i])
	{
		free(m[0][i]);
		i++;
	}
	if (m)
	{
		free(m[0]);
		*m = NULL;
	}
}

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

void	othercommands(t_data *data, t_mini *mini_cmd, DIR *dir)
{
	pid_t	pid;
	int		status;
	char	*e_path;

	e_path = find_exec(mini_cmd->full_cmd[0]);
	// printf("e_path:%s\n",e_path);
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
	pid = fork();
	if (pid == 0)
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
	free(e_path);
}

// if (!is_builtin(mini_cmd->full_cmd[0])
// 	&& (!access(mini_cmd->full_cmd[0]->full_path, F_OK)) || dir)
// 	data->prev_exit_stat = 126;
// else if (!is_builtin(n) && n->full_cmd)
// 	data->prev_exit_stat = 127;

void	parse_command(char **args, t_data *data, t_prompt *test)
{
	t_list	*cmd_node;
	t_mini	*mini_cmd;
	DIR		*dir;

	test->cmds = fill_nodes(args, data);
	if (test->cmds && test->cmds->next != NULL)
		execute_pipes(test->cmds, data);
	else
	{
		cmd_node = test->cmds;
		while (cmd_node)
		{
			mini_cmd = (t_mini *)cmd_node->content;
			if (mini_cmd && mini_cmd->full_cmd)
				dir = opendir(mini_cmd->full_cmd[0]);
			if (mini_cmd && mini_cmd->full_cmd && mini_cmd->full_cmd[0])
			{
				if (is_builtin(mini_cmd->full_cmd[0]))
					builtin(data, mini_cmd);
				else
					othercommands(data, mini_cmd, dir);
				check_and_update_shlvl(data, mini_cmd);
			}
			cmd_node = cmd_node->next;
		}
	}
	// ft_lstclear(&test->cmds, free_mini);
}
