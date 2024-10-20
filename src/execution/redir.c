/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aagdemir <aagdemir@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 13:57:55 by aagdemir          #+#    #+#             */
/*   Updated: 2024/10/20 21:46:45 by aagdemir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	get_fd(int oldfd, char *path, int flags[2], t_data *data)
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
	if (fd == -1)
	{
		perror(path);
		data->prev_exit_stat = 1;
	}
	return (fd);
}

void	outfile1(t_mini **node, char **args, int *i, t_data *data)
{
	int	flags[2];

	flags[0] = 1;
	flags[1] = 0;
	(*i)++;
	if (args[*i])
		(*node)->outfile = get_fd((*node)->outfile, args[*i], flags, data);
}

void	outfile2(t_mini **node, char **args, int *i, t_data *data)
{
	int	flags[2];

	flags[0] = 1;
	flags[1] = 1;
	if (args[++(*i)])
		(*node)->outfile = get_fd((*node)->outfile, args[*i], flags, data);
}

void	infile1(t_mini **node, char **args, int *i, t_data *data)
{
	int	flags[2];

	flags[0] = 0;
	flags[1] = 0;
	(*i)++;
	if (args[*i])
	{
		(*node)->infile = get_fd((*node)->infile, args[*i], flags, data);
		if ((*node)->infile == -1)
		{
			ft_putendl_fd("Error: Cannot open input file", 2);
			data->prev_exit_stat = 1;
			*i = -1;
		}
	}
}

void	infile2(t_mini **node, char **args, int *i, t_data *data)
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
		if ((*node)->infile == -1)
		{
			ft_putendl_fd("Error: Heredoc failed", 2);
			data->prev_exit_stat = 1;
			*i = -1;
		}
	}
}
