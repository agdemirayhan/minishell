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

char	**ft_extend_matrix(char **matrix, char *new_str)
{
	int		i;
	char	**new_matrix;

	i = 0;
	// Count existing elements in matrix
	while (matrix && matrix[i])
		i++;
	// Allocate space for the new matrix with one more element
	new_matrix = malloc(sizeof(char *) * (i + 2));
	if (!new_matrix)
		return (NULL);
	// Copy old matrix to the new one
	i = 0;
	while (matrix && matrix[i])
	{
		new_matrix[i] = matrix[i];
		i++;
	}
	// Add the new string
	new_matrix[i] = strdup(new_str);
	// strdup to allocate new memory for the string
	new_matrix[i + 1] = NULL; // NULL-terminate the array
	// Free the old matrix, but not the individual strings
	if (matrix)
		free(matrix);
	return (new_matrix);
}

// The get_params function with the addition of storing arguments into full_cmd.
static t_mini	*get_params(t_mini *node, char ***args, int *i)
{
	if ((*args)[*i])
	{
		printf("Processing argument: %s\n", (*args)[*i]);
		// Output redirection ">>" (append mode)
		if ((*args)[*i][0] == '>' && (*args)[*i + 1] && (*args)[*i
			+ 1][0] == '>')
		{
			printf("Detected append redirection >>\n");
			(*i)++; // Move to the file name
			node->outfile = open((*args)[*i], O_WRONLY | O_CREAT | O_APPEND,
					0644);
			if (node->outfile == -1)
				perror("Error opening file for append");
			else
				printf("File opened for append: %s\n", (*args)[*i]);
			node->append = 1;
		}
		// Output redirection ">"
		else if ((*args)[*i][0] == '>')
		{
			printf("Detected output redirection >\n");
			(*i)++; // Move to the file name
			node->outfile = open((*args)[*i], O_WRONLY | O_CREAT | O_TRUNC,
					0644);
			if (node->outfile == -1)
				perror("Error opening file for output");
			else
				printf("File opened for output: %s\n", (*args)[*i]);
			node->append = 0;
		}
		// Input redirection "<<"
		else if ((*args)[*i][0] == '<' && (*args)[*i + 1] && (*args)[*i
			+ 1][0] == '<')
		{
			printf("Detected heredoc redirection <<\n");
			(*i)++; // Move to the file name
			node->infile = open((*args)[*i], O_RDONLY);
			if (node->infile == -1)
				perror("Error opening file for heredoc");
			else
				printf("File opened for heredoc: %s\n", (*args)[*i]);
		}
		// Input redirection "<"
		else if ((*args)[*i][0] == '<')
		{
			printf("Detected input redirection <\n");
			(*i)++; // Move to the file name
			node->infile = open((*args)[*i], O_RDONLY);
			if (node->infile == -1)
				perror("Error opening file for input");
			else
				printf("File opened for input: %s\n", (*args)[*i]);
		}
		// Regular command or argument
		else if ((*args)[*i][0] != '|') // Ignore the pipe symbol here
		{
			printf("Detected command or argument: %s\n", (*args)[*i]);
			// Extend full_cmd (use ft_extend_matrix to store the argument)
			node->full_cmd = ft_extend_matrix(node->full_cmd, (*args)[*i]);
		}
		else // Handle unexpected pipe
		{
			printf("Unexpected pipe '|' encountered: %s\n", (*args)[*i]);
			*i = -2; // Temporary exit condition to avoid endless loop
			return (node);
		}
		(*i)++; // Move to the next argument for the next call
	}
	return (node);
}

t_stack	*print_list(t_stack *head)
{
	t_stack	*current;

	current = head;
	while (current)
	{
		// Cast the `void *` content to `char *` and print it
		printf("%s\n", (char *)current->content);
		current = current->next;
	}
	return (head);
}

/**
 * @brief Creates linked lists of commands split by pipes ('|').
 * Parses the input `args` array and creates separate linked
 * lists for each set of commands and arguments divided by pipes.
 * Returns an array of linked lists.
 * @param args Array of strings representing the parsed command input.
 * @return An array of linked lists, each representing a command chain.
 * The array ends with NULL.
 */
t_stack	**fill_nodes(char **args)
{
	int		i;
	int		list_index;
	t_stack	**cmds;

	i = 0;
	list_index = 0;
	cmds = malloc(sizeof(t_stack *) * 11); // Allocate space for up to 10 lists
	if (!cmds)
		return (NULL);
	// Initialize the command lists
	while (list_index < 10)
		cmds[list_index++] = NULL;
	list_index = 0;
	while (args[i])
	{
		t_mini *node; // Pointer to store the current node
		// Allocate and initialize a new t_mini node manually
		node = malloc(sizeof(t_mini));
		if (!node)
			return (NULL); // Handle memory allocation failure
		node->full_cmd = NULL;
		node->infile = -1;
		node->outfile = -1;
		node->next = NULL; // Ensure the next pointer is NULL initially
		// If encountering a pipe ('|'), start a new command list
		if (args[i][0] == '|' && args[i + 1] && args[i + 1][0])
		{
			i++;          // Skip the pipe
			list_index++; // Move to the next command list
		}
		// Process the arguments and redirections with get_params
		while (args[i] && args[i][0] != '|')
			node = get_params(node, &args, &i); // Process the current argument
		// Print full_cmd array
		if (node->full_cmd)
		{
			printf("full_cmd:");
			for (int j = 0; node->full_cmd[j]; j++)
			{
				printf("%s ", node->full_cmd[j]);
			}
			printf("\n");
		}
		else
		{
			printf("full_cmd is empty\n");
		}
		// Add the processed node to the current command list
		cmds[list_index] = insert_at_tail(cmds[list_index], (char *)node);
	}
	// Set the final list in cmds to NULL to mark the end
	cmds[list_index + 1] = NULL;
	print_cmds(&cmds[0]);
	return (cmds);
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
	t_stack	**cmds;

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