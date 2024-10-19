#include "minishell.h"

void	free_mini(void *content)
{
	t_mini	*mini_cmd;

	mini_cmd = (t_mini *)content;
	if (mini_cmd)
	{
		if (mini_cmd->full_cmd)
		{
			ft_free_matrix(&(mini_cmd->full_cmd));
		}
		free(mini_cmd);
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
	while (matrix && matrix[i])
		i++;
	new_matrix = malloc((i + 2) * sizeof(char *));
	if (!new_matrix)
		return (NULL);
	i = 0;
	while (matrix && matrix[i])
	{
		new_matrix[i] = matrix[i];
		i++;
	}
	new_matrix[i] = ft_strdup(new_entry);
	new_matrix[i + 1] = NULL;
	free(matrix);
	return (new_matrix);
}

int	is_redir(char *arg)
{
	if (!arg)
		return (0);
	if (ft_strcmp(arg, ">") == 0
		|| ft_strcmp(arg, ">>") == 0
		|| ft_strcmp(arg, "<") == 0
		|| ft_strcmp(arg, "<<") == 0)
	{
		return (1);
	}
	return (0);
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
