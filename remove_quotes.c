#include "minishell.h"

/**
 * @brief This function calculates the number of quotes (' and ")
 * that should be removed from the input string s1.
 * @return the length of the new str w/o quotes.
 */

void	newstr_len_helper(enum QuoteState *state, const char *s, int *length)
{
	if (*state == NO_QUOTE)
	{
		if (*s == '\'')
			*state = SINGLE_QUOTE;
		else if (*s == '"')
			*state = DOUBLE_QUOTE;
		else
			(*length)++;
	}
	else if (*state == SINGLE_QUOTE)
	{
		if (*s == '\'')
			*state = NO_QUOTE;
		else
			(*length)++;
	}
	else if (*state == DOUBLE_QUOTE)
	{
		if (*s == '"')
			*state = NO_QUOTE;
		else
			(*length)++;
	}
}

int	newstr_len(const char *s)
{
	enum QuoteState	state;
	int				length;

	state = NO_QUOTE;
	length = 0;
	while (*s)
	{
		newstr_len_helper(&state, s, &length);
		s++;
	}
	return (length);
}

typedef struct s_trim_info
{
	int				i;
	int				j;
	enum QuoteState	state;
	char			*trimmed;
}					t_trim_info;

int	handle_quotes(const char *s1, t_trim_info *info)
{
	if (info->state == NO_QUOTE)
	{
		if (s1[info->i] == '\'')
			info->state = SINGLE_QUOTE;
		else if (s1[info->i] == '"')
			info->state = DOUBLE_QUOTE;
	}
	else if ((info->state == SINGLE_QUOTE && s1[info->i] == '\'')
		|| (info->state == DOUBLE_QUOTE && s1[info->i] == '"'))
	{
		info->state = NO_QUOTE;
	}
	else
	{
		info->trimmed[info->j] = s1[info->i];
		info->j++;
	}
	info->i++;
	return (info->j);
}

int	strtrim_all_helper(const char *s1, t_trim_info *info)
{
	if (s1[info->i] == '\'' || s1[info->i] == '"')
		return (handle_quotes(s1, info));
	else
	{
		info->trimmed[info->j] = s1[info->i];
		info->j++;
		info->i++;
	}
	return (info->j);
}

char	*ft_strtrim_all(const char *s1)
{
	t_trim_info	info;
	int			new_len;

	if (!s1)
		return (NULL);
	info.i = 0;
	info.j = 0;
	info.state = NO_QUOTE;
	new_len = newstr_len(s1);
	info.trimmed = (char *)malloc(sizeof(char) * (new_len + 1));
	if (!info.trimmed)
		return (NULL);
	while (s1[info.i])
		strtrim_all_helper(s1, &info);
	info.trimmed[info.j] = '\0';
	return (info.trimmed);
}
