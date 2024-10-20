/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_nodes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aagdemir <aagdemir@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 21:34:34 by aagdemir          #+#    #+#             */
/*   Updated: 2024/10/20 21:34:40 by aagdemir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static t_list	*stop_fill(t_list *cmds, char **args)
{
	ft_lstclear(&cmds, free_content);
	ft_free_matrix(&args);
	return (NULL);
}

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

int	fill_nodes_errorhandler(char **args, t_list *cmds, int *i)
{
	if ((args[*i][0] == '|' && *i == 0) || (args[*i][0] == '|' && !args[*i
			+ 1]))
	{
		ft_putstr_fd("syntax error near unexpected token '|'\n", 2);
		ft_lstclear(&cmds, free_content);
		return (0);
	}
	if (is_redir(args[*i]) && (!args[*i + 1] || is_redir(args[*i + 1])))
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
	if (!is_redir(args[*i]))
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
