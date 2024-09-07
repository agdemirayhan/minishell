#include "minishell.h"

/**
 * @brief This function calculates the number of quotes (' and ")
 * that should be removed from the input string s1.
 * @return the length of the new str w/o quotes.
 */

int	newstr_len(const char *s)
{
	enum QuoteState	state;
	int				length;

	state = NO_QUOTE;
	length = 0;
	while (*s)
	{
		if (state == NO_QUOTE)
		{
			if (*s == '\'')
				state = SINGLE_QUOTE;
			else if (*s == '"')
				state = DOUBLE_QUOTE;
			else
				length++;
		}
		else if (state == SINGLE_QUOTE)
		{
			if (*s == '\'')
				state = NO_QUOTE;
			else
				length++;
		}
		else if (state == DOUBLE_QUOTE)
		{
			if (*s == '"')
				state = NO_QUOTE;
			else
				length++;
		}
		s++;
	}
	return (length);
}

/**
 * @brief This function creates a new string by removing all unescaped single
 * and double quotes from the input string s1.
 * @return the new str.
 */
char	*ft_strtrim_all(const char *s1)
{
	enum QuoteState	state;
	int				i;
	int				j;
	int				new_len;
	char			*trimmed;

	i = 0;
	state = NO_QUOTE;
	j = 0;
	if (!s1)
		return (NULL);
	new_len = newstr_len(s1);
	trimmed = (char *)malloc(sizeof(char) * (new_len + 1));
	if (!trimmed)
		return (NULL);
	while (s1[i])
	{
		if (state == NO_QUOTE)
		{
			if (s1[i] == '\'')
				state = SINGLE_QUOTE;
			else if (s1[i] == '"')
				state = DOUBLE_QUOTE;
			else
				trimmed[j++] = s1[i];
		}
		else if (state == SINGLE_QUOTE)
		{
			if (s1[i] == '\'')
				state = NO_QUOTE;
			else
				trimmed[j++] = s1[i];
		}
		else if (state == DOUBLE_QUOTE)
		{
			if (s1[i] == '"')
				state = NO_QUOTE;
			else
				trimmed[j++] = s1[i];
		}
		i++;
	}
	trimmed[j] = '\0';
	return (trimmed);
}

// int	main(void)
// {
// 	const char	*test_str = "echo \"hello there\" 'and more'";
// 	char		*result;

// 	result = ft_strtrim_all(test_str);
// 	printf("Trimmed string: %s\n", result);
// 	free(result);
// 	return (0);
// }
