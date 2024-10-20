/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_echo.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aagdemir <aagdemir@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 21:05:25 by aagdemir          #+#    #+#             */
/*   Updated: 2024/10/20 23:44:50 by aagdemir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*join_space(char **args)
{
	size_t	total_length;
	int		i;
	char	*result;

	total_length = 0;
	i = 0;
	while (args[i] != NULL)
	{
		total_length += ft_strlen(args[i]) + 1;
		i++;
	}
	result = malloc(total_length);
	if (result == NULL)
		return (NULL);
	result[0] = '\0';
	i = 0;
	while (args[i] != NULL)
	{
		ft_strlcat(result, args[i], total_length);
		if (args[i + 1] != NULL)
			ft_strlcat(result, " ", total_length);
		i++;
	}
	return (result);
}

void	b_echo(char **args)
{
	int		i;
	int		newline;
	char	*str;

	i = 1;
	newline = 1;
	str = NULL;
	if (!args[1] || !*(args[1]))
		return ((void)(printf("\n")));
	if (ft_strncmp(args[1], "-n", ft_strlen(args[1])) == 0)
	{
		newline = 0;
		i = 2;
	}
	str = join_space(&args[i]);
	if (newline)
		str = free_strjoin(str, "\n");
	printf("%s", str);
	free(str);
}
