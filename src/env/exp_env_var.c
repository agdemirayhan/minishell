/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_env_var.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aagdemir <aagdemir@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 22:10:35 by aagdemir          #+#    #+#             */
/*   Updated: 2024/10/21 23:27:15 by aagdemir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

typedef struct s_expand_state
{
	char	*result;
	size_t	result_len;
	int		in_single_quote;
	size_t	total_size;
	size_t	i;
}			t_expand_state;

void	*find_env_ref(t_env *env_list, char *name)
{
	t_env	*curr;

	curr = env_list;
	while (curr)
	{
		if (ft_strncmp(curr->name, name, ft_strlen(name)) == 0)
		{
			return (curr->value);
		}
		curr = curr->next;
	}
	return (NULL);
}

void	handle_single_quote(const char *input, t_expand_state *state)
{
	if (input[state->i] == '\'')
	{
		state->in_single_quote = !state->in_single_quote;
		state->total_size++;
		state->i++;
	}
}

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

void	handle_env_variable(const char *input, t_expand_state *state,
		t_data *data)
{
	char	*var_name;
	char	*var_value;
	size_t	start;
	char	*exit_stat;

	state->i++;
	if (input[state->i] == '?')
	{
		exit_stat = ft_itoa(data->prev_exit_stat);
		state->total_size += ft_strlen(exit_stat);
		free(exit_stat);
		state->i++;
	}
	else if (ft_strncmp(&input[state->i], "EMPTY", 5) == 0)
	{
		state->i += 5;
	}
	else
	{
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
}

void	handle_other_characters(const char *input, t_expand_state *state)
{
	state->total_size++;
	state->i++;
}

void	build_result_string(const char *input, t_expand_state *state,
		t_data *data)
{
	char	*exit_stat;
	size_t	start;
	char	*var_name;
	char	*var_value;

	state->i = 0;
	while (input[state->i] != '\0')
	{
		if (input[state->i] == '\'')
		{
			state->in_single_quote = !state->in_single_quote;
			state->result[state->result_len++] = input[state->i++];
		}
		else if (input[state->i] == '$' && input[state->i + 1] != '\0'
			&& !state->in_single_quote)
		{
			state->i++;
			if (input[state->i] == '?')
			{
				exit_stat = ft_itoa(data->prev_exit_stat);
				ft_strlcpy(state->result + state->result_len, exit_stat,
					ft_strlen(exit_stat) + 1);
				state->result_len += ft_strlen(exit_stat);
				free(exit_stat);
				state->i++;
			}
			else if (ft_strncmp(&input[state->i], "EMPTY", 5) == 0)
				state->i += 5;
			else
			{
				start = state->i;
				while (input[state->i] && (ft_isalnum(input[state->i])
						|| input[state->i] == '_'))
					state->i++;
				var_name = ft_strndup(input + start, state->i - start);
				var_value = find_env_ref(data->env_list, var_name);
				if (var_value)
				{
					ft_strlcpy(state->result + state->result_len, var_value,
						ft_strlen(var_value) + 1);
					state->result_len += ft_strlen(var_value);
				}
				else
				{
					ft_strlcpy(state->result + state->result_len, var_name,
						ft_strlen(var_name) + 1);
					state->result_len += ft_strlen(var_name);
				}
				free(var_name);
			}
		}
		else
			state->result[state->result_len++] = input[state->i++];
	}
}

char	*expand_env_vars(char *input, t_data *data)
{
	t_expand_state	state;

	state.total_size = 1;
	state.i = 0;
	state.result_len = 0;
	state.in_single_quote = 0;
	while (input[state.i] != '\0')
	{
		if (input[state.i] == '\'')
			handle_single_quote(input, &state);
		else if (input[state.i] == '$' && input[state.i + 1] != '\0'
			&& !state.in_single_quote)
			handle_env_variable(input, &state, data);
		else
			handle_other_characters(input, &state);
	}
	state.result = malloc(state.total_size);
	if (!state.result)
		return (NULL);
	build_result_string(input, &state, data);
	state.result[state.result_len] = '\0';
	return (state.result);
}
