/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_helpers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aagdemir <aagdemir@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 21:51:46 by aagdemir          #+#    #+#             */
/*   Updated: 2024/10/21 21:54:28 by aagdemir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	curr_loop(t_env *curr, char *value, char *name)
{
	if (value)
	{
		free(curr->value);
		curr->value = value;
	}
	free(name);
	return ;
}

void	export_error(t_data *data, char *name, char *value)
{
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd(name, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
	free(name);
	free(value);
	data->prev_exit_stat = 1;
	return ;
}

int	is_valid_var_name(const char *name)
{
	int	i;

	if (!name || !*name)
		return (0);
	if (!ft_isalpha(*name))
		return (0);
	i = 1;
	while (name[i] != '\0')
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (0);
		i++;
	}
	i = 0;
	while (name[i] != '\0')
	{
		if (ft_isalpha(name[i]))
			return (1);
		i++;
	}
	return (0);
}
