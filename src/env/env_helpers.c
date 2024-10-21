/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_helpers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aagdemir <aagdemir@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 22:02:56 by aagdemir          #+#    #+#             */
/*   Updated: 2024/10/21 22:03:07 by aagdemir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/**
 * @brief updates the value of the env variables
 */
void	update_env(t_env **env_list, char *name, char *value)
{
	t_env	*temp;

	temp = *env_list;
	while (temp != NULL)
	{
		if (ft_strncmp(temp->name, name, ft_strlen(name)) == 0)
		{
			free(temp->value);
			temp->value = ft_strdup(value);
			return ;
		}
		temp = temp->next;
	}
}

/**
 * @brief Prints the environment variables in the linked list
 */
void	print_env_list(t_env *env)
{
	while (env != NULL)
	{
		if (ft_strncmp(env->name, "_", 1) == 0 && !(ft_strncmp(env->name, "__",
					2) == 0))
			printf("_=/usr/bin/env\n");
		else
		{
			if (env->value)
				printf("%s=%s\n", env->name, env->value);
			else
				printf("%s\n", env->name);
		}
		env = env->next;
	}
}