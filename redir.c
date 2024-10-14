#include "minishell.h"

extern int	g_status;

int	get_fd(int oldfd, char *path, int flags[2])
{
	int	fd;

	// Close the old file descriptor if it's greater than 2
	if (oldfd > 2)
		close(oldfd);
	// If no path is provided, return an error (-1)
	if (!path)
		return (-1);
	// Open the file based on the flags
	if (flags[0] && flags[1])
		fd = open(path, O_CREAT | O_WRONLY | O_APPEND, 0666); // Append mode
	else if (flags[0] && !flags[1])
		fd = open(path, O_CREAT | O_WRONLY | O_TRUNC, 0666); // Overwrite mode
	else if (!flags[0] && oldfd != -1)
		fd = open(path, O_RDONLY); // Read-only mode
	else
		fd = oldfd;
	if (fd == -1)
	{
		perror(path); // Prints a detailed error message to stderr
		g_status = 1;
		// Set global status to indicate an error (usually 1 is for general errors)
	}
	return (fd);
}

void	outfile1(t_mini **node, char **args, int *i)
{
	int	flags[2];

	flags[0] = 1;
	flags[1] = 0;
	(*i)++;
	if (args[*i])
		(*node)->outfile = get_fd((*node)->outfile, args[*i], flags);
}

void	outfile2(t_mini **node, char **args, int *i)
{
	int	flags[2];

	flags[0] = 1;
	flags[1] = 1;
	if (args[++(*i)])
		(*node)->outfile = get_fd((*node)->outfile, args[*i], flags);
}

void	infile1(t_mini **node, char **args, int *i)
{
	int	flags[2];

	flags[0] = 0; // Reading from the file (not writing)
	flags[1] = 0;
	(*i)++;
	// Check if the file exists in args[*i]
	if (args[*i])
	{
		(*node)->infile = get_fd((*node)->infile, args[*i], flags);
		if ((*node)->infile == -1)
		{
			ft_putendl_fd("Error: Cannot open input file", 2);
			g_status = 1; // Set an appropriate status code
			*i = -1;      // Stop the processing of further commands
		}
	}
}

void	infile2(t_mini **node, char **args, int *i)
{
	char	*del;
	char	*str[2];

	str[0] = NULL;
	str[1] = NULL;
	del = NULL;
	(*i)++;
	if (args[(*i)])
	{
		del = args[*i];
		(*node)->infile = heredoc_handler(str, del);
		// Handle the case where heredoc_handler returns an error
		if ((*node)->infile == -1)
		{
			ft_putendl_fd("Error: Heredoc failed", 2);
			g_status = 1; // Set global status to indicate an error
			*i = -1;      // Stop further command execution
		}
	}
}

void	get_redir(t_mini **node, char **args, int *i)
{
	// Check if the current argument is a redirection operator
	if (args[*i] && *i != -1)
	{
		// Double redirection '>>' (append to output file)
		if (args[*i][0] == '>' && args[*i][1] && args[*i][1] == '>')
		{
			outfile2(node, args, i);
		}
		// Single redirection '>' (overwrite output file)
		else if (args[*i][0] == '>')
		{
			outfile1(node, args, i);
		}
		// Double redirection '<<' (heredoc)
		else if (args[*i][0] == '<' && args[*i][1] && args[*i][1] == '<')
		{
			infile2(node, args, i);
		}
		// Single redirection '<' (input file)
		else if (args[*i][0] == '<')
		{
			infile1(node, args, i);
		}
	}
	// If *i == -1, return early to prevent further execution
	if (*i == -1)
		return ;
}
