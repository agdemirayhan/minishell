/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   e_export.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aagdemir <aagdemir@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 16:52:59 by aagdemir          #+#    #+#             */
/*   Updated: 2024/10/21 21:53:53 by aagdemir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	execute_export(char **argv, t_data *data)
{
	int		i;
	t_env	*curr;

	if (!argv[1])
	{
		curr = data->env_list;
		while (curr)
		{
			if (curr->value)
				printf("declare -x %s=\"%s\"\n", curr->name, curr->value);
			else
				printf("declare -x %s\n", curr->name);
			curr = curr->next;
		}
		return ;
	}
	i = 1;
	while (argv[i])
	{
		export_var(&argv[i], &(data->env_list), data);
		i++;
	}
}

void	parse_and_validate_var(char **argv, char **name, char **value,
		t_data *data)
{
	char	*equal;

	equal = ft_strchr(*argv, '=');
	if (equal)
	{
		*name = ft_strndup(*argv, equal - *argv);
		*value = ft_strdup(equal + 1);
	}
	else
	{
		*name = ft_strdup(*argv);
		*value = NULL;
	}
	if (!is_valid_var_name(*name))
		export_error(data, *name, *value);
}

void	update_env_list(t_env **env_list, char *name, char *value)
{
	t_env	*curr;
	t_env	*new_node;

	curr = *env_list;
	while (curr)
	{
		if (ft_strncmp(curr->name, name, ft_strlen(name)) == 0)
		{
			curr_loop(curr, name, value);
			return ;
		}
		curr = curr->next;
	}
	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return ;
	new_node->name = name;
	new_node->value = value;
	new_node->next = *env_list;
	*env_list = new_node;
}

void	export_var(char **argv, t_env **env_list, t_data *data)
{
	char	*name;
	char	*value;

	value = NULL;
	parse_and_validate_var(argv, &name, &value, data);
	if (data->prev_exit_stat == 1)
		return ;
	update_env_list(env_list, name, value);
}
