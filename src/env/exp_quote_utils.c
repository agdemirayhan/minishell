/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_quote_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aagdemir <aagdemir@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 09:28:50 by aagdemir          #+#    #+#             */
/*   Updated: 2024/10/22 09:55:03 by aagdemir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_single_quote(const char *input, t_expand_state *state)
{
	if (input[state->i] == '\'')
	{
		state->in_single_quote = !state->in_single_quote;
		state->total_size++;
		state->i++;
	}
}

void	handle_other_characters(t_expand_state *state)
{
	state->total_size++;
	state->i++;
}

void	handle_single_quote_in_result(const char *input, t_expand_state *state)
{
	state->in_single_quote = !state->in_single_quote;
	state->result[state->result_len++] = input[state->i++];
}

void	process_character_for_result(const char *input, t_expand_state *state,
		t_data *data)
{
	if (input[state->i] == '\'')
		handle_single_quote_in_result(input, state);
	else if (input[state->i] == '$' && input[state->i + 1] != '\0'
		&& !state->in_single_quote)
	{
		state->i++;
		expand_variable_in_result(input, state, data);
	}
	else
		state->result[state->result_len++] = input[state->i++];
}
