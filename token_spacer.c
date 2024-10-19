#include "minishell.h"

void	counter_helper(char *s, int i[2], enum QuoteState *quote_state,
		char *new_str)
{
	if (s[i[0]] == '"' && (*quote_state != SINGLE_QUOTE))
	{
		if (*quote_state == NO_QUOTE)
			*quote_state = DOUBLE_QUOTE;
		else
			*quote_state = NO_QUOTE;
		if (new_str != NULL)
			new_str[i[1]++] = s[i[0]];
	}
	else if (s[i[0]] == '\'' && (*quote_state != DOUBLE_QUOTE))
	{
		if (*quote_state == NO_QUOTE)
			*quote_state = SINGLE_QUOTE;
		else
			*quote_state = NO_QUOTE;
		if (new_str != NULL)
			new_str[i[1]++] = s[i[0]];
	}
}

void	token_counter(char *s, int i[2], enum QuoteState *quote_state)
{
	while (s[i[0]] != '\0')
	{
		counter_helper(s, i, quote_state, NULL);
		if (*quote_state == NO_QUOTE && (s[i[0]] == '\\' || s[i[0]] == '<'
				|| s[i[0]] == '|' || s[i[0]] == '>'))
		{
			if (i[0] > 0 && s[i[0] - 1] != ' ')
				i[1]++;
			i[1]++;
			if (s[i[0] + 1] != ' ')
				i[1]++;
		}
		else
			i[1]++;
		i[0]++;
	}
}

void	modifier_helper(char *s, int i[2], char *new_str)
{
	if (i[0] > 0 && s[i[0] - 1] != ' ')
		new_str[i[1]++] = ' ';
	new_str[i[1]++] = s[i[0]];
	if ((s[i[0] + 1] == '<' && s[i[0]] == '<') || (s[i[0] + 1] == '>'
			&& s[i[0]] == '>'))
		new_str[i[1]++] = s[++i[0]];
	if (s[i[0] + 1] != ' ')
		new_str[i[1]++] = ' ';
}

void	token_modifier(char *s, int i[2], enum QuoteState *quote_state,
		char *new_str)
{
	while (s[i[0]] != '\0')
	{
		if (s[i[0]] == '"' && *quote_state != SINGLE_QUOTE)
		{
			if (*quote_state == NO_QUOTE)
				*quote_state = DOUBLE_QUOTE;
			else
				*quote_state = NO_QUOTE;
			new_str[i[1]++] = s[i[0]];
		}
		else if (s[i[0]] == '\'' && *quote_state != DOUBLE_QUOTE)
		{
			if (*quote_state == NO_QUOTE)
				*quote_state = SINGLE_QUOTE;
			else
				*quote_state = NO_QUOTE;
			new_str[i[1]++] = s[i[0]];
		}
		else if (*quote_state == NO_QUOTE && (s[i[0]] == '\\' || s[i[0]] == '<'
					|| s[i[0]] == '|' || s[i[0]] == '>'))
			modifier_helper(s, i, new_str);
		else
			new_str[i[1]++] = s[i[0]];
		i[0]++;
	}
}

/**
 * @brief This function puts spaces between tokens if necessary
 * @return a string
 */
char	*token_spacer(char *s)
{
	char *new_str;
	enum QuoteState quote_state;
	int i[2];

	i[0] = 0;
	i[1] = 0;
	quote_state = NO_QUOTE;
	token_counter(s, i, &quote_state);
	new_str = malloc(sizeof(char) * (i[1] + 1));
	if (!new_str)
		return (NULL);
	i[0] = 0;
	i[1] = 0;
	quote_state = NO_QUOTE;
	token_modifier(s, i, &quote_state, new_str);
	new_str[i[1]] = '\0';
	return (new_str);
}