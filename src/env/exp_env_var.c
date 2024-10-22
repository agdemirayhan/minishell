/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_env_var.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aagdemir <aagdemir@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 22:10:35 by aagdemir          #+#    #+#             */
/*   Updated: 2024/10/22 09:36:19 by aagdemir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_special_variables(const char *input, size_t *i,
		size_t *total_size, t_data *data)
{
	char	*exit_stat;

	if (input[*i] == '?')
	{
		exit_stat = ft_itoa(data->prev_exit_stat);
		*total_size += ft_strlen(exit_stat);
		free(exit_stat);
		(*i)++;
	}
	else if (ft_strncmp(&input[*i], "EMPTY", 5) == 0)
		(*i) += 5;
}

void	handle_regular_env_variable(const char *input, size_t *i,
		size_t *total_size, t_data *data)
{
	char	*var_name;
	char	*var_value;
	size_t	start;

	start = *i;
	while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
		(*i)++;
	var_name = ft_strndup(input + start, *i - start);
	var_value = find_env_ref(data->env_list, var_name);
	if (var_value)
		*total_size += ft_strlen(var_value);
	else
		*total_size += *i - start;
	free(var_name);
}

void	handle_special_var(const char *input, t_expand_state *state,
		t_data *data)
{
	char	*exit_stat;

	if (input[state->i] == '?')
	{
		exit_stat = ft_itoa(data->prev_exit_stat);
		state->total_size += ft_strlen(exit_stat);
		free(exit_stat);
		state->i++;
	}
	else if (ft_strncmp(&input[state->i], "EMPTY", 5) == 0)
		state->i += 5;
}

void	handle_regular_var(const char *input, t_expand_state *state,
		t_data *data)
{
	char	*var_name;
	char	*var_value;
	size_t	start;

	start = state->i;
	while (input[state->i] && (ft_isalnum(input[state->i])
			|| input[state->i] == '_'))
		state->i++;
	var_name = ft_strndup(input + start, state->i - start);
	var_value = find_env_ref(data->env_list, var_name);
	if (var_value)
		state->total_size += ft_strlen(var_value);
	else
		state->total_size += state->i - start;
	free(var_name);
}

void	handle_env_variable(const char *input, t_expand_state *state,
		t_data *data)
{
	state->i++;
	if (input[state->i] == '?' || ft_strncmp(&input[state->i], "EMPTY", 5) == 0)
		handle_special_var(input, state, data);
	else
		handle_regular_var(input, state, data);
}
