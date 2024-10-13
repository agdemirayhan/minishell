#include "minishell.h"

extern int	g_status;
void	remove_delimiter(char **str, const char *del)
{
	char	*del_pos;
	size_t	del_len;

	del_len = ft_strlen(del);
	del_pos = ft_strnstr(*str, del, ft_strlen(*str));
	// Find the delimiter in the string
	if (del_pos != NULL)
	{
		del_len = ft_strlen(del);
		if (del_pos[del_len] == '\n')
			// Check if the delimiter is followed by a newline
			del_len++;
		ft_memmove(del_pos, del_pos + del_len, ft_strlen(del_pos + del_len)
			+ 1);
	}
}
char	*heredoc_str(char *s[2], size_t len, char *lim)
{
	char	*temp;

	if (s[1] == NULL)
		s[1] = ft_strdup(""); // Initialize the result string
	while (g_status != 130 && (!s[0] || ft_strncmp(s[0], lim, len)
			|| ft_strlen(lim) != len))
	{
		if (s[0] != NULL)
			free(s[0]); // Free s[0] after each readline
		s[0] = readline("heredoc> ");
		if (s[0] == NULL)
		{
			fprintf(stderr, "Error: readline failed.\n");
			return (s[1]); // Return the accumulated result
		}
		if (ft_strncmp(s[0], lim, len) == 0 && ft_strlen(lim) == len)
		{
			break ;
		}
		// Append a newline to the current line of input
		temp = s[0];
		s[0] = ft_strjoin(s[0], "\n"); // Create a new string with a newline
		free(temp);                    // Free the old s[0] (stored in temp)
		// Append the current input to the result string (s[1])
		temp = s[1];
		s[1] = ft_strjoin(s[1], s[0]);
		free(temp); // Free the old result string (s[1])
	}
	free(s[0]); // Free the final s[0] after the loop
	return (s[1]);
}

int	heredoc_handler(char *str[2], char *del)
{
	int	fd[2];

	g_status = 0;
	if (pipe(fd) == -1)
	{
		fprintf(stderr, "Error creating pipe.\n");
		return (-1);
	}
	str[1] = heredoc_str(str, ft_strlen(del), del);
	remove_delimiter(&str[1], del);
	// Remove the delimiter from the final result
	write(fd[1], str[1], ft_strlen(str[1])); // Write the content to the pipe
	free(str[1]);
	close(fd[1]);
	signal(SIGINT, handle_signals);
	signal(SIGQUIT, handle_signals);
	if (g_status == 130)
	{
		close(fd[0]);
		return (-1);
	}
	return (fd[0]);
}
