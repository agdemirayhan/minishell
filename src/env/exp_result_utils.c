/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_result_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aagdemir <aagdemir@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 09:29:34 by aagdemir          #+#    #+#             */
/*   Updated: 2024/10/22 09:56:03 by aagdemir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

void	expand_var_helper(const char *input, t_expand_state *state,
		t_data *data, size_t start)
{
	char	*var_name;
	char	*var_value;

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

void	expand_variable_in_result(const char *input, t_expand_state *state,
		t_data *data)
{
	char	*exit_stat;
	size_t	start;

	start = 0;
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
		expand_var_helper(input, state, data, start);
}

void	build_result_string(const char *input, t_expand_state *state,
		t_data *data)
{
	state->i = 0;
	while (input[state->i] != '\0')
	{
		process_character_for_result(input, state, data);
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
			handle_other_characters(&state);
	}
	state.result = malloc(state.total_size);
	if (!state.result)
		return (NULL);
	build_result_string(input, &state, data);
	state.result[state.result_len] = '\0';
	return (state.result);
}
