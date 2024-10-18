#include "minishell.h"

// MORE TESTS

static t_mini	*mini_init(void)
{
	t_mini	*mini;

	mini = malloc(sizeof(t_mini));
	if (!mini)
		return (NULL);
	mini->full_cmd = NULL;
	mini->full_path = NULL;
	mini->infile = STDIN_FILENO;
	mini->outfile = STDOUT_FILENO;
	return (mini);
}

void	token_helper(char *s, int i[2], enum QuoteState *quote_state,
		char *new_str)
{
	if (s[i[0]] == '"' && (*quote_state != SINGLE_QUOTE))
	{
		if (*quote_state == NO_QUOTE)
			*quote_state = DOUBLE_QUOTE;
		else
			*quote_state = NO_QUOTE;
		if (new_str != NULL)
			new_str[i[1]++] = s[i[0]];
	}
	else if (s[i[0]] == '\'' && (*quote_state != DOUBLE_QUOTE))
	{
		if (*quote_state == NO_QUOTE)
			*quote_state = SINGLE_QUOTE;
		else
			*quote_state = NO_QUOTE;
		if (new_str != NULL)
			new_str[i[1]++] = s[i[0]];
	}
}

void	token_counter(char *s, int i[2], enum QuoteState *quote_state)
{
	while (s[i[0]] != '\0')
	{
		token_helper(s, i, quote_state, NULL);
		if (*quote_state == NO_QUOTE && (s[i[0]] == '\\' || s[i[0]] == '<'
				|| s[i[0]] == '|' || s[i[0]] == '>'))
		{
			if (i[0] > 0 && s[i[0] - 1] != ' ')
				i[1]++;
			i[1]++;
			if (s[i[0] + 1] != ' ')
				i[1]++;
		}
		else
			i[1]++;
		i[0]++;
	}
}

void	token_modifier(char *s, int i[2], enum QuoteState *quote_state,
		char *new_str)
{
	while (s[i[0]] != '\0')
	{
		if (s[i[0]] == '"' && *quote_state != SINGLE_QUOTE)
		{
			if (*quote_state == NO_QUOTE)
				*quote_state = DOUBLE_QUOTE;
			else
				*quote_state = NO_QUOTE;
			new_str[i[1]++] = s[i[0]];
		}
		else if (s[i[0]] == '\'' && *quote_state != DOUBLE_QUOTE)
		{
			if (*quote_state == NO_QUOTE)
				*quote_state = SINGLE_QUOTE;
			else
				*quote_state = NO_QUOTE;
			new_str[i[1]++] = s[i[0]];
		}
		else if (*quote_state == NO_QUOTE && (s[i[0]] == '\\' || s[i[0]] == '<'
				|| s[i[0]] == '|' || s[i[0]] == '>'))
		{
			if (i[0] > 0 && s[i[0] - 1] != ' ')
				new_str[i[1]++] = ' ';
			new_str[i[1]++] = s[i[0]];
			if ((s[i[0] + 1] == '<' && s[i[0]] == '<') || (s[i[0] + 1] == '>'
					&& s[i[0]] == '>'))
				new_str[i[1]++] = s[++i[0]];
			if (s[i[0] + 1] != ' ')
				new_str[i[1]++] = ' ';
		}
		else
			new_str[i[1]++] = s[i[0]];
		i[0]++;
	}
}

/**
 * @brief This function puts spaces between tokens if necessary
 * @return a string
 */
char	*token_spacer(char *s)
{
	char			*new_str;
	enum QuoteState	quote_state;
	int				i[2];

	i[0] = 0;
	i[1] = 0;
	quote_state = NO_QUOTE;
	token_counter(s, i, &quote_state);
	new_str = malloc(sizeof(char) * (i[1] + 1));
	if (!new_str)
		return (NULL);
	i[0] = 0;
	i[1] = 0;
	quote_state = NO_QUOTE;
	token_modifier(s, i, &quote_state, new_str);
	new_str[i[1]] = '\0';
	return (new_str);
}

static int	ft_count_words(const char *s, char *c, int i[2])
{
	enum QuoteState	quote_state;

	quote_state = NO_QUOTE;
	while (s[i[0]] != '\0')
	{
		while ((!ft_strchr(c, s[i[0]]) || quote_state != NO_QUOTE)
			&& s[i[0]] != '\0')
		{
			if (quote_state == NO_QUOTE && (s[i[0]] == '\'' || s[i[0]] == '\"'))
			{
				if (s[i[0]] == '\'')
					quote_state = SINGLE_QUOTE;
				else if (s[i[0]] == '\"')
					quote_state = DOUBLE_QUOTE;
			}
			else if ((quote_state == SINGLE_QUOTE && s[i[0]] == '\'')
					|| (quote_state == DOUBLE_QUOTE && s[i[0]] == '\"'))
				quote_state = NO_QUOTE;
			i[0]++;
		}
		i[1]++;
		while (ft_strchr(c, s[i[0]]) && s[i[0]] != '\0')
			i[0]++;
	}
	return (i[1]);
}

void	split_helper(char *del, const char *s, int *i[3],
		enum QuoteState *quote_state)
{
	*quote_state = NO_QUOTE;
	while (ft_strchr(del, s[*i[0]]) && s[*i[0]] != '\0')
		(*i[0])++;
	*i[1] = *i[0];
	while ((!ft_strchr(del, s[*i[0]]) || *quote_state != NO_QUOTE)
		&& s[*i[0]] != '\0')
	{
		if (*quote_state == NO_QUOTE && (s[*i[0]] == '\'' || s[*i[0]] == '\"'))
		{
			if (s[*i[0]] == '\'')
				*quote_state = SINGLE_QUOTE;
			else
				*quote_state = DOUBLE_QUOTE;
		}
		else if ((*quote_state == SINGLE_QUOTE && s[*i[0]] == '\'')
				|| (*quote_state == DOUBLE_QUOTE && s[*i[0]] == '\"'))
			*quote_state = NO_QUOTE;
		(*i[0])++;
	}
}

/**
 * @brief This function splits words without removing the quotes
 * @return array of splitted strings.
 */
char	**split_with_quotes(const char *s, char *del)
{
	char			**arr;
	int				words_len;
	int				s_len;
	enum QuoteState	quote_state;
	int				i[3] = {0};
	int				counts[2] = {0};

	words_len = ft_count_words(s, del, counts);
	arr = malloc((words_len + 1) * sizeof(char *));
	if (arr == NULL)
		return (NULL);
	s_len = ft_strlen(s);
	while (s[i[0]] != '\0')
	{
		split_helper(del, s, (int *[]){&i[0], &i[1], &i[2]}, &quote_state);
		if (i[1] >= s_len)
			arr[i[2]++] = "\0";
		else
			arr[i[2]++] = ft_substr(s, i[1], i[0] - i[1]);
	}
	if (quote_state != NO_QUOTE)
		return (ft_putstr_fd("Error: Unclosed quotes detected\n",
				STDERR_FILENO), free(arr), NULL);
	arr[words_len] = NULL;
	return (arr);
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

int	is_redirection(char *arg)
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

static t_list	*stop_fill(t_list *cmds, char **args)
{
	ft_lstclear(&cmds, free_content);
	ft_free_matrix(&args);
	return (NULL);
}

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

int	fill_nodes_errorhandler(char **args, t_list *cmds, int *i)
{
	if ((args[*i][0] == '|' && *i == 0) || (args[*i][0] == '|' && !args[*i
			+ 1]))
	{
		ft_putstr_fd("syntax error near unexpected token '|'\n", 2);
		ft_lstclear(&cmds, free_content);
		return (0);
	}
	if (is_redirection(args[*i]) && (!args[*i + 1] || is_redirection(args[*i
				+ 1])))
	{
		ft_putstr_fd("syntax error near unexpected token '", 2);
		ft_putstr_fd(args[*i], 2);
		ft_putstr_fd("'\n", 2);
		ft_lstclear(&cmds, free_content);
		return (0);
	}
	return (1);
}

void	fill_nodes_main(char **args, t_list **cmds, int *i, t_data *data)
{
	t_list	*last_cmd;
	t_mini	*first_mini;

	last_cmd = ft_lsttraverse(*cmds);
	if (*i == 0 || (args[*i][0] == '|' && args[*i + 1] && args[*i + 1][0]))
	{
		*i += (args[*i][0] == '|');
		ft_lst_insertattail(cmds, ft_lst_newlist(mini_init()));
		last_cmd = ft_lsttraverse(*cmds);
	}
	first_mini = (t_mini *)last_cmd->content;
	if (!is_redirection(args[*i]))
		first_mini->full_cmd = ft_extend_matrix(first_mini->full_cmd, args[*i]);
	get_redir(&first_mini, args, i, data);
}

t_list	*fill_nodes(char **args, t_data *data)
{
	t_list	*cmds;
	int		i;

	i = -1;
	cmds = NULL;
	while (args[++i])
	{
		if (!fill_nodes_errorhandler(args, cmds, &i))
			return (NULL);
		fill_nodes_main(args, &cmds, &i, data);
		if (i < 0)
			return (stop_fill(cmds, args));
		if (!args[i])
			break ;
	}
	return (ft_free_matrix(&args), cmds);
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
