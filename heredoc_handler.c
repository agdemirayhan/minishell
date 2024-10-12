#include "minishell.h"

extern int	g_status;

void	remove_delimiter(char **str, const char *del)
{
	char	*del_pos;
	size_t	del_len;

	del_len = ft_strlen(del);
	del_pos = ft_strnstr(*str, del, ft_strlen(*str));
	if (del_pos != NULL)
	{
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

	if (s[1] == NULL)
		s[1] = ft_strdup("");
	result = s[1];
	while (g_status != 130 && (!s[0] || ft_strncmp(s[0], lim, len)
			|| ft_strlen(lim) != len))
	{
		if (s[0] != NULL)
			free(s[0]);
		s[0] = readline("heredoc> ");
		if (s[0] == NULL)
		{
			fprintf(stderr, "Error: readline failed.\n");
			return (result);
		}
		temp = s[0];
		s[0] = ft_strjoin(s[0], "\n");
		free(temp);
		temp = s[1];
		s[1] = ft_strjoin(s[1], s[0]);
		// free(temp);
		len = ft_strlen(s[0]) - 1;
		signal(SIGINT, handle_signals);
		signal(SIGQUIT, handle_signals);
	}
	free(s[0]);
	return (s[1]);
}

int	heredoc_handler(char *str[2], char *del)
{
	int fd[2];

	g_status = 0;
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
	signal(SIGINT, handle_signals);
	signal(SIGQUIT, handle_signals);
	printf("g_status:%d\n", g_status);
	if (g_status == 130)
	{
		close(fd[0]);
		return (-1);
	}
	return (fd[0]);
}