/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_cd.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aagdemir <aagdemir@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 21:27:43 by aagdemir          #+#    #+#             */
/*   Updated: 2024/10/20 23:44:57 by aagdemir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_dir(char **args)
{
	char	*home;
	char	*home1;

	if (!args[1] || ft_strlen(args[1]) == 0)
	{
		home = getenv("HOME");
		if (!home)
			return (NULL);
		return (ft_strdup(home));
	}
	if (ft_strncmp(args[1], "~", 1) == 0)
	{
		home1 = getenv("HOME");
		if (!home1)
			return (NULL);
		return (ft_strjoin(home1, args[1] + 1));
	}
	return (ft_strdup(args[1]));
}

void	b_cd(char **args, t_data *data)
{
	char	*dir;

	dir = get_dir(args);
	if (!dir)
		return ;
	update_old_pwd(&(data->env_list));
	if (chdir(dir) != 0)
	{
		perror("minishell");
		data->prev_exit_stat = 1;
	}
	else
		update_pwd(&(data->env_list));
	free(dir);
}
