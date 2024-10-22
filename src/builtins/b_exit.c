/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_exit.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aagdemir <aagdemir@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 20:59:02 by aagdemir          #+#    #+#             */
/*   Updated: 2024/10/22 09:59:22 by aagdemir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	check_exit_num(char *arg, int *exit_code)
{
	int	i;
	int	num;

	i = 0;
	while (arg[i] == ' ' || arg[i] == '\t')
		i++;
	num = i;
	while (arg[num] != '\0')
	{
		if (arg[num] != '-' && arg[num] != '+' && !ft_isdigit(arg[num]))
		{
			ft_putstr_fd("exit: numeric argument required\n", 2);
			return (1);
		}
		num++;
	}
	*exit_code = ft_atoi(&arg[i]);
	if (*exit_code > 255)
		*exit_code = *exit_code % 256;
	if (*exit_code < 0)
		*exit_code = 256 + *exit_code;
	return (0);
}

void	handle_mini_count(t_data *data, int previous_shlvl)
{
	char	*new_value;

	if (data->mini_count == 1)
	{
		clean_up(data);
		exit(data->prev_exit_stat);
	}
	else
	{
		data->mini_count--;
		if (previous_shlvl > 0)
		{
			new_value = ft_itoa(previous_shlvl);
			if (new_value)
			{
				update_env(&(data->env_list), "SHLVL", new_value);
				free(new_value);
			}
		}
	}
}

void	handle_shlvl_and_exit(t_data *data)
{
	int				previous_shlvl;
	t_shlvl_node	*temp;

	if (data->shlvl_history)
	{
		previous_shlvl = data->shlvl_history->shlvl_value;
		temp = data->shlvl_history;
		data->shlvl_history = data->shlvl_history->next;
		free(temp);
		handle_mini_count(data, previous_shlvl);
	}
	else
		handle_mini_count(data, -1);
}

int	handle_exit_args(char **args, int *exit_code, t_data *data)
{
	if (check_exit_num(args[1], exit_code))
	{
		data->prev_exit_stat = 255;
		return (1);
	}
	if (args[2] != NULL)
	{
		ft_putstr_fd("exit: too many arguments\n", 2);
		data->prev_exit_stat = 1;
		return (1);
	}
	data->prev_exit_stat = *exit_code;
	return (0);
}

void	b_exit(char **args, t_data *data)
{
	int	exit_code;

	exit_code = 0;
	printf("exit\n");
	if (args[1] != NULL)
	{
		if (handle_exit_args(args, &exit_code, data))
			return ;
	}
	handle_shlvl_and_exit(data);
}
