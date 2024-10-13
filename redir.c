#include "minishell.h"

extern int	g_status;

int	get_fd(int oldfd, char *path, int flags[2])
{
	int	fd;

	if (oldfd > 2)
		close(oldfd);
	if (!path)
		return (-1);
	if (flags[0] && flags[1])
		fd = open(path, O_CREAT | O_WRONLY | O_APPEND, 0666);
	else if (flags[0] && !flags[1])
		fd = open(path, O_CREAT | O_WRONLY | O_TRUNC, 0666);
	else if (!flags[0] && oldfd != -1)
		fd = open(path, O_RDONLY);
	else
		fd = oldfd;
	return (fd);
}

void	outfile(t_mini **node, char **args, int *i, int o)
{
	int	flags[2];

	if (o == 1)
	{
		flags[0] = 1;
		flags[1] = 0;
		(*i)++;
		if (args[*i])
			(*node)->outfile = get_fd((*node)->outfile, args[*i], flags);
	}
	else
	{
		flags[0] = 1;
		flags[1] = 1;
		if (args[++(*i)])
			(*node)->outfile = get_fd((*node)->outfile, args[*i], flags);
	}
}

void	infile1(t_mini **node, char **args, int *i)
{
	int	flags[2];

	flags[0] = 0;
	flags[1] = 0;
	(*i)++;
	if (args[*i])
		(*node)->infile = get_fd((*node)->infile, args[*i], flags);
}

void	infile2(t_mini **node, char **args, int *i)
{
	char	*del;
	char	*str[2];

	str[0] = NULL;
	str[1] = NULL;
	del = NULL;
	printf("YES1\n");
	(*i)++;
	if (args[(*i)])
	{
		del = args[*i];
		(*node)->infile = heredoc_handler(str, del);
	}
	if (!args[*i] || (*node)->infile == -1)
	{
		*i = -1;
		if ((*node)->infile != -1)
		{
			ft_putendl_fd("test", 2);
			g_status = 2;
		}
	}
}

void	get_redir(t_mini **node, char **args, int *i)
{
	//
	if (args[*i])
	{
		if (args[*i][0] == '>' && args[*i][1] && args[*i][1] == '>')
			outfile(node, args, i, 2);
		else if (args[*i][0] == '>')
			outfile(node, args, i, 1);
		else if (args[*i][0] == '<' && args[*i][1] && args[*i][1] == '<')
			infile2(node, args, i);
		else if (args[*i][0] == '<')
			infile1(node, args, i);
	}
}

