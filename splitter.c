#include "minishell.h"

void	handle_quote_state(const char *s, int *i, enum QuoteState *q_state)
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

static int	ft_count_words(const char *s, char *c, enum QuoteState *q_state)
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
		enum QuoteState *q_state)
{
	*q_state = NO_QUOTE;
	while (ft_strchr(del, s[*i[0]]) && s[*i[0]] != '\0')
		(*i[0])++;
	*i[1] = *i[0];
	while ((!ft_strchr(del, s[*i[0]]) || *q_state != NO_QUOTE)
		&& s[*i[0]] != '\0')
	{
		if (*q_state == NO_QUOTE && (s[*i[0]] == '\'' || s[*i[0]] == '\"'))
		{
			if (s[*i[0]] == '\'')
				*q_state = SINGLE_QUOTE;
			else
				*q_state = DOUBLE_QUOTE;
		}
		else if ((*q_state == SINGLE_QUOTE && s[*i[0]] == '\'')
			|| (*q_state == DOUBLE_QUOTE && s[*i[0]] == '\"'))
			*q_state = NO_QUOTE;
		(*i[0])++;
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
	int				s_len;
	enum QuoteState	quote_state;
	int				i[3];

	words_len = ft_count_words(s, del, &quote_state);
	if (words_len < 0 || quote_state != NO_QUOTE)
		return (ft_putstr_fd("Error: Unclosed quotes detected\n",
				STDERR_FILENO), NULL);
	arr = malloc((words_len + 1) * sizeof(char *));
	if (arr == NULL)
		return (NULL);
	s_len = ft_strlen(s);
	while (s[i[0]] != '\0')
	{
		split_helper(del, s, (int *[]){&i[0], &i[1], &i[2]}, &quote_state);
		if (i[1] < s_len)
			arr[i[2]++] = ft_substr(s, i[1], i[0] - i[1]);
		else
			break ;
	}
	arr[i[2]] = NULL;
	return (arr);
}
