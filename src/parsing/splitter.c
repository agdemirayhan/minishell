/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   splitter.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aagdemir <aagdemir@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 13:44:06 by aagdemir          #+#    #+#             */
/*   Updated: 2024/10/22 09:58:38 by aagdemir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_quote_state(const char *s, int *i, enum e_QuoteState *q_state)
{
	if (*q_state == NO_QUOTE && (s[*i] == '\'' || s[*i] == '\"'))
	{
		if (s[*i] == '\'')
			*q_state = SINGLE_QUOTE;
		else
			*q_state = DOUBLE_QUOTE;
	}
	else if ((*q_state == SINGLE_QUOTE && s[*i] == '\'')
		|| (*q_state == DOUBLE_QUOTE && s[*i] == '\"'))
		*q_state = NO_QUOTE;
	(*i)++;
}

static int	ft_count_words(const char *s, char *c, enum e_QuoteState *q_state)
{
	int	i[2];

	i[0] = 0;
	i[1] = 0;
	*q_state = NO_QUOTE;
	while (s[i[0]] != '\0')
	{
		while (ft_strchr(c, s[i[0]]) && *q_state == NO_QUOTE)
			i[0]++;
		if (s[i[0]] == '\0')
			break ;
		i[1]++;
		while ((!ft_strchr(c, s[i[0]]) || *q_state != NO_QUOTE)
			&& s[i[0]] != '\0')
			handle_quote_state(s, &i[0], q_state);
	}
	return (i[1]);
}

void	split_helper(char *del, const char *s, int *i[3],
		enum e_QuoteState *quote_state)
{
	*quote_state = NO_QUOTE;
	while (ft_strchr(del, s[*i[0]]) && s[*i[0]] != '\0')
		(*i[0])++;
	*i[1] = *i[0];
	while ((!ft_strchr(del, s[*i[0]]) || *quote_state != NO_QUOTE)
		&& s[*i[0]] != '\0')
	{
		if (*quote_state == NO_QUOTE && (s[*i[0]] == '\'' || s[*i[0]] == '\"'))
		{
			if (s[*i[0]] == '\'')
				*quote_state = SINGLE_QUOTE;
			else
				*quote_state = DOUBLE_QUOTE;
		}
		else if ((*quote_state == SINGLE_QUOTE && s[*i[0]] == '\'')
			|| (*quote_state == DOUBLE_QUOTE && s[*i[0]] == '\"'))
			*quote_state = NO_QUOTE;
		(*i[0])++;
	}
}

void	process_split(const char *s, char *del, char **arr,
		t_split_state *state)
{
	int	s_len;

	s_len = ft_strlen(s);
	while (s[state->i[0]] != '\0')
	{
		split_helper(del, s, (int *[]){&state->i[0], &state->i[1],
			&state->i[2]}, &state->quote_state);
		if (state->i[1] >= s_len)
		{
			arr[state->i[2]++] = "\0";
		}
		else
		{
			arr[state->i[2]++] = ft_substr(s, state->i[1], state->i[0]
					- state->i[1]);
		}
	}
}

/**
 * @brief This function splits words without removing the quotes
 * @return array of splitted strings.
 */
char	**split_with_quotes(const char *s, char *del)
{
	char			**arr;
	int				words_len;
	t_split_state	state;

	state.quote_state = NO_QUOTE;
	state.i[0] = 0;
	state.i[1] = 0;
	state.i[2] = 0;
	state.counts[0] = 0;
	state.counts[1] = 0;
	words_len = ft_count_words(s, del, &state.quote_state);
	arr = malloc((words_len + 1) * sizeof(char *));
	if (arr == NULL)
		return (NULL);
	process_split(s, del, arr, &state);
	if (state.quote_state != NO_QUOTE)
	{
		ft_putstr_fd("Error: Unclosed quotes detected\n", STDERR_FILENO);
		free(arr);
		return (NULL);
	}
	arr[words_len] = NULL;
	return (arr);
}
