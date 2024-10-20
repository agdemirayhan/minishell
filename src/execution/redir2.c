/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aagdemir <aagdemir@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 13:58:59 by aagdemir          #+#    #+#             */
/*   Updated: 2024/10/20 23:45:45 by aagdemir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	get_redir(t_mini **node, char **args, int *i, t_data *data)
{
	if (args[*i] && *i != -1)
	{
		if (args[*i][0] == '>' && args[*i][1] && args[*i][1] == '>')
			outfile2(node, args, i, data);
		else if (args[*i][0] == '>')
			outfile1(node, args, i, data);
		else if (args[*i][0] == '<' && args[*i][1] && args[*i][1] == '<')
			infile2(node, args, i, data);
		else if (args[*i][0] == '<')
			infile1(node, args, i, data);
	}
	if (*i == -1)
		return ;
}
