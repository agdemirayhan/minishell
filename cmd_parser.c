#include "minishell.h"

// MORE TESTS

/**
 * @brief This function put spaces between tokens if necessary
 * @return a string
 */
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
	/*this needs to be removed its triggering me :( */
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
	if (quote_state != NO_QUOTE)
	{
		fprintf(stderr, "Error: Unclosed quotes detected\n");
		free(arr); // Free the allocated memory if an error occurs
		return (NULL);
	}
	arr[words_len] = NULL;
	return (arr);
}

t_stack	*fill_nodes(char **args)
{
	int	i;

	t_stack *cmds; // Only one pointer is needed to track the head of the list
	i = 0;
	cmds = NULL; // Initialize the head to NULL, since the list starts empty
	while (args[i])
	{
		printf("args:[%i]:%s\n", i, args[i]);
		cmds = insert_at_tail(cmds, args[i]);
			// Update the head after each insertion
		i++;
	}
	print_cmds(cmds); // Assuming print_cmds prints the entire list
	return (cmds);    // Return the head of the list
}

void	parse_command(char *input, t_data *data)
{
	char	**args;
	char	*new_str;
	char	*expanded_str;
	int		i;
	char	*trimmed_arg;
	int		has_heredoc;
	char	*delimiter;
	char	*heredoc_pos;
	t_stack	*cmds;

	has_heredoc = 0;
	delimiter = NULL;
	i = 0;
	new_str = token_spacer(input);
	if (!new_str)
		return ;
	expanded_str = expand_env_vars(new_str, data);
	free(new_str);
	args = split_with_quotes(expanded_str, " ");
	if (!args)
		return ;
	free(expanded_str);
	while (args[i])
	{
		trimmed_arg = ft_strtrim_all(args[i]);
		free(args[i]);
		args[i] = trimmed_arg;
		printf("args[%d]:%s\n", i, args[i]);
		i++;
	}
	if (args[0] == NULL)
		return ;
	cmds = fill_nodes(args);
	print_cmds(cmds);
	// Check if the command is a builtin or external command
	if (is_builtin(args[0]))
	{
		execute_builtin(args, data);
	}
	else
	{
		execute_command(args);
	}
	// Free the argument array
	i = 0;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
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