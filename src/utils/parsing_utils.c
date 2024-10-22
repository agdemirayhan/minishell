/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aagdemir <aagdemir@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 20:33:55 by aagdemir          #+#    #+#             */
/*   Updated: 2024/10/22 09:59:31 by aagdemir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	process_input(char *input, t_data *data, char ***args)
{
	char	*new_str;
	char	*expanded_str;
	char	*trimmed_expanded_str;

	new_str = token_spacer(input);
	if (!new_str)
		return (-1);
	expanded_str = expand_env_vars(new_str, data);
	free(new_str);
	trimmed_expanded_str = ft_strtrim(expanded_str, " \t\n");
	free(expanded_str);
	*args = split_with_quotes(trimmed_expanded_str, " ");
	free(trimmed_expanded_str);
	if (!(*args) || (*args)[0] == NULL)
	{
		free(*args);
		return (-1);
	}
	return (0);
}

void	trim_arguments(char **args)
{
	int		i;
	char	*trimmed_arg;

	i = 0;
	while (args[i])
	{
		trimmed_arg = ft_strtrim_all(args[i]);
		free(args[i]);
		args[i] = trimmed_arg;
		i++;
	}
}

int	parsing_helpers(char *input, t_data *data, char ***args)
{
	int	ret;

	ret = process_input(input, data, args);
	if (ret == -1)
		return (-1);
	ret = handle_syntax_error(*args);
	if (ret == -1)
		return (-1);
	trim_arguments(*args);
	return (0);
}

void	parsing_handler(char *input, t_data *data)
{
	t_prompt	test;
	char		**args;

	test.cmds = NULL;
	if (parsing_helpers(input, data, &args) == -1)
		return ;
	parse_command(args, data, &test);
}
