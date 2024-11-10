/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   e_export.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aagdemir <aagdemir@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 16:52:59 by aagdemir          #+#    #+#             */
/*   Updated: 2024/11/10 13:32:23 by aagdemir         ###   ########.fr       */
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

void	export_var_helpnew(t_env **env_list, char *value,
		char *name)
{
	t_env	*new_node;
	t_env	*curr;

	curr = *env_list;
	while (curr)
	{
		if (ft_strncmp(curr->name, name, ft_strlen(name)) == 0)
		{
			if (value)
			{
				free(curr->value);
				curr->value = value;
			}
			free(name);
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
	char	*equal;
	char	*name;
	char	*value;

	equal = ft_strchr(*argv, '=');
	value = NULL;
	if (equal)
	{
		name = ft_strndup(*argv, equal - *argv);
		value = ft_strdup(equal + 1);
	}
	else
		name = ft_strdup(*argv);
	if (!is_valid_var_name(name))
	{
		ft_putstr_fd("minishell: export: `", 2);
		ft_putstr_fd(name, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		free(name);
		free(value);
		data->prev_exit_stat = 1;
		return ;
	}
	export_var_helpnew(env_list, value, name);
}
