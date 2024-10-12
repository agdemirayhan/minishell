/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aagdemir <aagdemir@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 17:37:37 by aagdemir          #+#    #+#             */
/*   Updated: 2024/10/12 17:37:38 by aagdemir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

void	parse_command2(char *input, t_data *data, t_prompt *test)
{
	char		*new_str;
	char		*expanded_str;
	char		**args;
	char		*trimmed_arg;
	int i;

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
