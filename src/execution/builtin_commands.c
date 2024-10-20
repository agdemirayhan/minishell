/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aagdemir <aagdemir@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 21:29:52 by aagdemir          #+#    #+#             */
/*   Updated: 2024/10/20 23:38:29 by aagdemir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_builtin(char *command)
{
	return (ft_strncmp(command, "echo", ft_strlen("echo") + 1) == 0
		|| ft_strncmp(command, "cd", ft_strlen("cd") + 1) == 0
		|| ft_strncmp(command, "pwd", ft_strlen("pwd") + 1) == 0
		|| ft_strncmp(command, "export", ft_strlen("export") + 1) == 0
		|| ft_strncmp(command, "unset", ft_strlen("unset") + 1) == 0
		|| ft_strncmp(command, "env", ft_strlen("env") + 1) == 0
		|| ft_strncmp(command, "exit", ft_strlen("exit") + 1) == 0);
}

char	*free_strjoin(char *str1, const char *str2)
{
	size_t	new_length;
	char	*result;

	new_length = ft_strlen(str1) + ft_strlen(str2) + 1;
	result = malloc(new_length);
	if (result == NULL)
	{
		free(str1);
		return (NULL);
	}
	ft_strlcpy(result, str1, new_length);
	ft_strlcat(result, str2, new_length);
	free(str1);
	return (result);
}

void	execute_builtin(char **args, t_data *data)
{
	if (ft_strncmp(args[0], "cd", ft_strlen("cd")) == 0)
		b_cd(args, data);
	else if (ft_strncmp(args[0], "exit", ft_strlen("exit")) == 0)
		b_exit(args, data);
	else if (ft_strncmp(args[0], "pwd", ft_strlen("pwd")) == 0)
		b_pwd();
	else if (ft_strncmp(args[0], "echo", ft_strlen("echo")) == 0)
		b_echo(args);
	else if (ft_strncmp(args[0], "env", ft_strlen("env")) == 0)
		print_env_list(data->env_list);
	else if (ft_strncmp(args[0], "export", ft_strlen("export")) == 0)
		execute_export(args, data);
	else if (ft_strncmp(args[0], "unset", ft_strlen("unset")) == 0)
		execute_unset(args, data);
}

void	clean_up(t_data *data)
{
	t_shlvl_node	*current;
	t_shlvl_node	*next;

	current = data->shlvl_history;
	free_env_list(&(data->env_list));
	if (data->shlvl_history)
	{
		while (current)
		{
			next = current->next;
			free(current);
			current = next;
		}
		data->shlvl_history = NULL;
	}
}
