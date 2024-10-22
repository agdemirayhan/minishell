/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   e_env.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aagdemir <aagdemir@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 21:57:00 by aagdemir          #+#    #+#             */
/*   Updated: 2024/10/22 09:35:03 by aagdemir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	append_env_node(t_env **head, t_env **curr, t_env *new_node)
{
	if (*head == NULL)
	{
		*head = new_node;
		*curr = *head;
	}
	else
	{
		(*curr)->next = new_node;
		*curr = (*curr)->next;
	}
}

void	handle_special_cases(t_env *node)
{
	if (ft_strncmp(node->name, "SHLVL", 6) == 0)
	{
		free(node->value);
		node->value = ft_strdup("2");
	}
	else if (ft_strncmp(node->name, "_", 1) == 0)
	{
		free(node->value);
		node->value = ft_strdup("/usr/bin/env");
	}
}

t_env	*init_new_env_node(char *env_str)
{
	t_env	*new_node;
	char	*equal;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return (NULL);
	equal = ft_strchr(env_str, '=');
	if (equal)
	{
		new_node->name = ft_strndup(env_str, equal - env_str);
		new_node->value = ft_strdup(equal + 1);
	}
	else
	{
		new_node->name = ft_strdup(env_str);
		new_node->value = NULL;
	}
	new_node->next = NULL;
	return (new_node);
}

t_env	*init_env_list(char **envp)
{
	t_env	*head;
	t_env	*curr;
	t_env	*new_node;
	int		i;

	head = NULL;
	curr = NULL;
	i = 0;
	while (envp[i] != NULL)
	{
		new_node = init_new_env_node(envp[i]);
		if (!new_node)
			return (NULL);
		handle_special_cases(new_node);
		append_env_node(&head, &curr, new_node);
		i++;
	}
	return (head);
}

void	free_env_list(t_env **env)
{
	t_env	*current;
	t_env	*next;

	current = *env;
	while (current != NULL)
	{
		next = current->next;
		free(current->name);
		free(current->value);
		free(current);
		current = next;
	}
	*env = NULL;
}
