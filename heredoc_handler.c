#include "minishell.h"

void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}

void	ft_putstr_fd(char *s, int fd)
{
	while (*s)
	{
		ft_putchar_fd(*s, fd);
		s++;
	}
}

void	ft_putendl_fd(char *s, int fd)
{
	ft_putstr_fd(s, fd);
	ft_putchar_fd('\n', fd);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	if (n == 0)
		return (0);
	while (s1[i] == s2[i] && s1[i] != '\0')
	{
		if (i < n - 1)
			i++;
		else
			return (0);
	}
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

extern int	g_status;

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*join;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (!s1 || !s2)
		return (NULL);
	join = (char *)malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (!join)
		return (NULL);
	while (s1[i] != '\0')
	{
		join[i] = s1[i];
		i++;
	}
	while (s2[j] != '\0')
		join[i++] = s2[j++];
	join[i] = '\0';
	return (join);
}
char	*heredoc_str(char *s[2], size_t len, char *lim)
{
	char	*temp;
	char	*result;

	result = s[1];
	while ((!s[0] || ft_strncmp(s[0], lim, len) || ft_strlen(lim) != len))
	{
		temp = s[1];
		s[1] = ft_strjoin(s[1], s[0]);
		free(temp);
		free(s[0]);
		s[0] = readline("heredoc> ");
		if (s[0] == NULL)
		{
			fprintf(stderr, "Error: readline failed.\n");
			return (result); // Return the accumulated text
		}
		temp = s[0];
		s[0] = ft_strjoin(s[0], "\n");
		free(temp);
		len = ft_strlen(s[0]) - 1;
	}
	free(s[0]);
	// Check if s[1] is NULL before returning
	if (s[1] == NULL)
	{
		fprintf(stderr, "Error: s[1] is NULL.\n");
		return (NULL);
	}
	return (s[1]);
}

int	heredoc_handler(char *str[2], char *del[2])
{
	int	fd[2];

	// Initialize the file descriptors
	if (pipe(fd) == -1)
	{
		fprintf(stderr, "Error creating pipe.\n"); // no fprintffing
		return (-1);
	}
	str[1] = heredoc_str(str, 0, del[0]);
	write(fd[1], str[1], ft_strlen(str[1]));
	free(str[1]);
	close(fd[1]);
	return (fd[0]);
}


// int	main(void)
// {
// 	char *str[2] = {NULL, NULL};
// 	// Array to hold input strings
// 	char *aux[2] = {"END", "Warning: Unexpected end of input"};
// 	// Delimiter and warning message

// 	// Call get_here_doc and store the read end of the pipe
// 	int read_fd = heredoc_handler(str, aux);
// 	if (read_fd == -1)
// 	{
// 		fprintf(stderr, "Error occurred while reading here document.\n");
// 		return (1);
// 	}

// 	// Read the accumulated input from the pipe
// 	char buffer[1024];
// 	ssize_t bytes_read;
// 	while ((bytes_read = read(read_fd, buffer, sizeof(buffer) - 1)) > 0)
// 	{
// 		buffer[bytes_read] = '\0'; // Null-terminate the buffer
// 		printf("Received input:\n%s", buffer);
// 	}
// 	close(read_fd);

// 	return (0);
// }