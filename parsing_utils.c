/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aagdemir <aagdemir@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 17:37:37 by aagdemir          #+#    #+#             */
/*   Updated: 2024/10/13 08:48:10 by aagdemir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	parse_command2(char *input, t_data *data, t_prompt *test)
{
	char	*new_str;
	char	*expanded_str;
	char	**args;
	char	*trimmed_arg;
	int		i;

	new_str = token_spacer(input);
	if (!new_str)
		return ;
	expanded_str = expand_env_vars(new_str, data);
	free(new_str);
	args = split_with_quotes(expanded_str, " ");
	free(expanded_str);
	if (!args || args[0] == NULL)
		return (free(args));
	i = 0;
	while (args[i])
	{
		printf("args:%s\n", args[i]);
		trimmed_arg = ft_strtrim_all(args[i]);
		args[i] = trimmed_arg;
		i++;
	}
	(*test).cmds = fill_nodes(args);
}

t_list	*handle_node_creation(t_list **cmds, char **args, int *i)
{
	t_list	*last_cmd;

	last_cmd = ft_lsttraverse(*cmds);
	if (*i == 0 || (args[*i][0] == '|' && args[*i + 1] && args[*i + 1][0]))
	{
		*i += (args[*i][0] == '|');
		ft_lst_insertattail(cmds, ft_lst_newlist(mini_init()));
		last_cmd = ft_lsttraverse(*cmds);
	}
	return (last_cmd);
}

static t_list	*stop_fill(t_list *cmds, char **args)
{
	ft_lstclear(&cmds, free_content);
	free_matrix(&args);
	return (NULL);
}

t_list	*fill_nodes(char **args)
{
	t_list	*cmds;
	t_list	*last_cmd;
	int		i;
	t_mini	*mini;

	i = -1;
	cmds = NULL;
	while (args[++i])
	{
		last_cmd = handle_node_creation(&cmds, args, &i);
		mini = (t_mini *)last_cmd->content;
		if (!is_redirection(args[i]))
			mini->full_cmd = extend_matrix(mini->full_cmd, args[i]);
		get_redir(&mini, args, &i);
		if (i < 0)
			return (stop_fill(cmds, args));
		if (!args[i])
			break ;
	}
	return (cmds);
}
