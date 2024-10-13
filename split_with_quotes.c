/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_with_quotes.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aagdemir <aagdemir@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 09:40:18 by aagdemir          #+#    #+#             */
/*   Updated: 2024/10/13 09:49:13 by aagdemir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_count_words(const char *s, char *c, int i[2])
{
	enum QuoteState	quote_state;

	quote_state = NO_QUOTE;
	while (s[i[0]] != '\0')
	{
		while ((!ft_strchr(c, s[i[0]]) || quote_state != NO_QUOTE)
			&& s[i[0]] != '\0')
		{
			if (quote_state == NO_QUOTE && (s[i[0]] == '\'' || s[i[0]] == '\"'))
			{
				if (s[i[0]] == '\'')
					quote_state = SINGLE_QUOTE;
				else if (s[i[0]] == '\"')
					quote_state = DOUBLE_QUOTE;
			}
			else if ((quote_state == SINGLE_QUOTE && s[i[0]] == '\'')
					|| (quote_state == DOUBLE_QUOTE && s[i[0]] == '\"'))
				quote_state = NO_QUOTE;
			i[0]++;
		}
		i[1]++;
		while (ft_strchr(c, s[i[0]]) && s[i[0]] != '\0')
			i[0]++;
	}
	return (i[1]);
}

char	**split_with_quotes(const char *s, char *del)
{
	char **arr;
	int words_len;
	int s_len;
	enum QuoteState quote_state;
	int i[3] = {0};
	int counts[2] = {0};

	quote_state = NO_QUOTE;
	words_len = ft_count_words(s, del, counts);
	arr = malloc((words_len + 1) * sizeof(char *));
	if (arr == NULL)
		return (NULL);
	s_len = ft_strlen(s);

	while (s[i[0]] != '\0')
	{
		while (ft_strchr(del, s[i[0]]) && s[i[0]] != '\0')
			i[0]++;
		i[1] = i[0];

		while ((!ft_strchr(del, s[i[0]]) || quote_state != NO_QUOTE)
			&& s[i[0]] != '\0')
		{
			if (quote_state == NO_QUOTE && (s[i[0]] == '\'' || s[i[0]] == '\"'))
			{
				if (s[i[0]] == '\'')
					quote_state = SINGLE_QUOTE;
				else
					quote_state = DOUBLE_QUOTE;
			}

			else if ((quote_state == SINGLE_QUOTE && s[i[0]] == '\'')
					|| (quote_state == DOUBLE_QUOTE && s[i[0]] == '\"'))
				quote_state = NO_QUOTE;
			i[0]++;
		}
		if (i[1] >= s_len)
			arr[i[2]++] = "\0";
		else
			arr[i[2]++] = ft_substr(s, i[1], i[0] - i[1]);
	}
	if (quote_state != NO_QUOTE)
	{
		fprintf(stderr, "Error: Unclosed quotes detected\n");
		free(arr);
		return (NULL);
	}
	arr[words_len] = NULL;
	return (arr);
}