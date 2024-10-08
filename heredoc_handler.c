#include "minishell.h"

extern int	g_status;

void	remove_delimiter(char **str, const char *del)
{
	char	*del_pos;
	size_t	del_len;

	del_pos = ft_strnstr(*str, del, del_len);
	if (del_pos != NULL)
	{
		del_len = ft_strlen(del);
		if (del_pos[del_len] == '\n')
			del_len++;
		ft_memmove(del_pos, del_pos + del_len, ft_strlen(del_pos + del_len)
			+ 1);
	}
}

char	*heredoc_str(char *s[2], size_t len, char *lim)
{
	char	*temp;
	char	*result;

	signal(SIGINT, handle_signals);
	signal(SIGQUIT, SIG_IGN);
	if (s[1] == NULL)
		s[1] = ft_strdup(""); // Duplicate empty string
	if (s[1] == NULL)
	{
		fprintf(stderr, "Error: Memory allocation failed.\n");
		return (NULL); // Handle memory allocation failure
	}
	result = s[1];
	while ((!s[0] || ft_strncmp(s[0], lim, len) || ft_strlen(lim) != len))
	{
		free(s[0]);
		s[0] = readline("heredoc> ");
		if (s[0] == NULL)
		{
			fprintf(stderr, "Error: readline failed.\n");
			return (result); // Return current result if readline fails
		}
		// Safely concatenate the newline to s[0]
		temp = ft_strjoin(s[0], "\n");
		if (temp == NULL) // Check for memory allocation failure
		{
			free(s[0]);
			fprintf(stderr, "Error: Memory allocation failed.\n");
			return (result); // Return current result on failure
		}
		free(s[0]);
		// Free the original s[0] before assigning the new concatenated value
		s[0] = temp;
		// Safely concatenate the input line to the result
		temp = ft_strjoin(s[1], s[0]);
		if (temp == NULL) // Check for memory allocation failure
		{
			free(s[0]); // Free the current input string
			free(s[1]); // Free the accumulated result string
			fprintf(stderr, "Error: Memory allocation failed.\n");
			return (result); // Return current result on failure
		}
		// free(s[1]); // Free the old result string before updating it
		s[1] = temp;
		len = ft_strlen(s[0]) - 1;
	}
	free(s[0]); // Free the last input line after exiting the loop
	return (s[1]);
}

int	heredoc_handler(char *str[2], char *del)
{
	int fd[2];

	if (pipe(fd) == -1)
	{
		fprintf(stderr, "Error creating pipe.\n");
		return (-1);
	}
	str[1] = heredoc_str(str, 0, del);
	remove_delimiter(&str[1], del);
	write(fd[1], str[1], ft_strlen(str[1]));
	free(str[1]);
	close(fd[1]);
	return (fd[0]);
}