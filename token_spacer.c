/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_spacer.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aagdemir <aagdemir@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 09:39:46 by aagdemir          #+#    #+#             */
/*   Updated: 2024/10/13 09:39:47 by aagdemir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

int	count_new_string_size(char *s)
{
	int	i;
	int	j;

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
	return (j);
}

void	fill_new_spaced_string(char *new_str, char *s)
{
	int	i;
	int	j;

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
				new_str[j++] = s[++i];
			if (s[i + 1] != ' ')
				new_str[j++] = ' ';
		}
		else
			new_str[j++] = s[i];
		i++;
	}
	new_str[j] = '\0';
}

char	*token_spacer(char *s)
{
	char	*new_str;
	int		new_str_size;

	new_str_size = count_new_string_size(s);
	new_str = malloc(sizeof(char) * (new_str_size + 1));
	if (!new_str)
		return (NULL);
	fill_new_spaced_string(new_str, s);
	return (new_str);
}
