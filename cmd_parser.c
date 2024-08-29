#include "minishell.h"

// MORE TESTS

// char	*space_terminator(char *s)
// {
// 	char	*new_str;
// 	int		i;
// 	int		j;

// 	i = 0;
// 	j = 0;
// 	while (s[i] != '\0')
// 	{
// 		while (s[i] == ' ')
// 			i++;
// 		i++;
// 		j++;
// 	}
// 	new_str = malloc(sizeof(char) * (j + 1));
// 	i = 0;
// 	j = 0;
// 	while (s[i] != '\0')
// 	{
// 		while (s[i] == ' ')
// 			i++;
// 		new_str[j++] = s[i++];
// 	}
// 	new_str[j] = '\0';
// 	return (new_str);
// }

char	*token_spacer(char *s)
{
	char	*new_str;
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (s[i] != '\0')
	{
		if (s[i] == '\\' || s[i] == '<' || s[i] == '|' || s[i] == '>')
		{
			if (i > 0 && s[i - 1] != ' ')
				j++;
			j++;
			if (s[i + 1] != ' ')
				j++;
		}
		else
			j++;
		i++;
	}
	new_str = malloc(sizeof(char) * (j + 1));
	if (!new_str)
		return (NULL);
	i = 0;
	j = 0;
	while (s[i] != '\0')
	{
		if (s[i] == '\\' || s[i] == '<' || s[i] == '|' || s[i] == '>')
		{
			if (i > 0 && s[i - 1] != ' ')
				new_str[j++] = ' ';
			new_str[j++] = s[i];
			if ((s[i + 1] == '<' && s[i] == '<') || (s[i + 1] == '>'
					&& s[i] == '>'))
			{
				new_str[j++] = s[++i];
			}
			if (s[i + 1] != ' ')
				new_str[j++] = ' ';
		}
		else
		{
			new_str[j++] = s[i];
		}
		i++;
	}
	new_str[j] = '\0';
	return (new_str);
}

static int	ft_count_words(const char *s, char *c, int i[2])
{
	enum QuoteState	quote_state;

	quote_state = NO_QUOTE;
	printf("ft_count_words: %s\n", s);
	while (s[i[0]] != '\0')
	{
		// Check if not a delimiter or within quotes
		while ((!ft_strchr(c, s[i[0]]) || quote_state != NO_QUOTE)
			&& s[i[0]] != '\0')
		{
			// Toggle quote state when encountering quotes
			if (quote_state == NO_QUOTE && (s[i[0]] == '\'' || s[i[0]] == '\"'))
			{
				if (s[i[0]] == '\'')
					quote_state = SINGLE_QUOTE;
				else if (s[i[0]] == '\"')
					quote_state = DOUBLE_QUOTE;
			}
			else if ((quote_state == SINGLE_QUOTE && s[i[0]] == '\'')
					|| (quote_state == DOUBLE_QUOTE && s[i[0]] == '\"'))
			{
				quote_state = NO_QUOTE;
			}
			i[0]++;
		}
		i[1]++;
		// Skip delimiters to avoid infinite loops
		while (ft_strchr(c, s[i[0]]) && s[i[0]] != '\0')
			i[0]++;
	}
	return (i[1]);
}

char	**split_with_quotes(const char *s, char *del)
{
	char			**arr;
	int				words_len;
	int				s_len;
	enum QuoteState	quote_state;
	int				i[3] = {0};
	int				counts[2] = {0};

	quote_state = NO_QUOTE;
	words_len = ft_count_words(s, del, counts);
	arr = malloc((words_len + 1) * sizeof(char *));
	if (arr == NULL)
		return (NULL);
	s_len = ft_strlen(s);
	// Iterate over the string to split words
	while (s[i[0]] != '\0')
	{
		// Skip delimiter characters
		while (ft_strchr(del, s[i[0]]) && s[i[0]] != '\0')
			i[0]++;
		i[1] = i[0];
		// Traverse the word, considering quotes
		while ((!ft_strchr(del, s[i[0]]) || quote_state != NO_QUOTE)
			&& s[i[0]] != '\0')
		{
			// Enter quote state if a quote is encountered
			if (quote_state == NO_QUOTE && (s[i[0]] == '\'' || s[i[0]] == '\"'))
			{
				if (s[i[0]] == '\'')
					quote_state = SINGLE_QUOTE;
				else
					quote_state = DOUBLE_QUOTE;
			}
			// Exit quote state if the matching quote is encountered again
			else if ((quote_state == SINGLE_QUOTE && s[i[0]] == '\'')
					|| (quote_state == DOUBLE_QUOTE && s[i[0]] == '\"'))
				quote_state = NO_QUOTE;
			i[0]++;
		}
		// Add the word to the array, handle empty case properly
		if (i[1] >= s_len)
			arr[i[2]++] = "\0";
		else
			arr[i[2]++] = ft_substr(s, i[1], i[0] - i[1]);
	}
	arr[words_len] = NULL;
	return (arr);
}

void	parse_command(char *input)
{
	char	**args;
	char	*new_str;
	int		i;

	i = 0;
	new_str = token_spacer(input);
	if (!new_str)
		return ;
	args = split_with_quotes(new_str, " ");
	while (args[i])
	{
		printf("args[%d]:%s\n", i, args[i]);
		i++;
	}
	if (args[0] == NULL)
		return ;
	if (is_builtin(args[0]))
	{
		execute_builtin(args);
	}
	else
	{
		execute_command(args);
	}
	// we must free the args array.
}

// int	main(void)
// {
// 	char input[] = "echo '\"Hello, \\\"World\\\"\" and 'it\\'s a test";
// 	char *processed = process_quotes(input);

// 	if (processed)
// 	{
// 		printf("Processed: %s\n", processed);
// 		free(processed);
// 	}

// 	return (0);
// }