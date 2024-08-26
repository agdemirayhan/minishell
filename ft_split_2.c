/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aagdemir <aagdemir@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 15:17:10 by msoklova          #+#    #+#             */
/*   Updated: 2024/08/25 12:41:34 by aagdemir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

static int	count_words(const char *s, char c)
{
	int	co;
	int	i;

	co = 0;
	i = 0;
	if (c == '\0')
		return (1);
	while (*s)
	{
		if (*s == '-' && *(s - 1) == ' ')
			co--;
		if (*s == c)
			i = 0;
		else if (i == 0)
		{
			co++;
			i = 1;
		}
		s++;
	}
	return (co);
}

static char	*duplicate_word(const char *s, char c)
{
	size_t	len;
	char	*dup;

	len = 0;
	while (s[len] && s[len] != c)
		len++;
	if (s[len + 1] == '-')
	{
		len++;
		while (s[len] && s[len] != c)
			len++;
	}
	printf("%zu\n", len);
	dup = (char *)malloc((len + 1) * sizeof(char));
	if (!dup)
		return (NULL);
	ft_strlcpy(dup, s, len + 1);
	return (dup);
}

int	ft_free(size_t i, char **result)
{
	while (i > 0)
	{
		free(result[i - 1]);
		i--;
	}
	free(result);
	return (0);
}

// This breaks norm. needs to be in 2 functions
char	**ft_split_2(char const *s, char c)
{
	char **result;
	size_t i;

	printf("%s\n",s);
	result = (char **)malloc((count_words(s, c) + 1) * sizeof(char *));
	if (!result)
		return (NULL);
	i = 0;
	while (*s)
	{
		if (*s == c)
			s++;
		else
		{
			result[i] = duplicate_word(s, c);
			if (!result[i++])
			{
				ft_free(i, result);
				return (NULL);
			}
			while (*s && *s != c)
				s++;
			if (*(s + 1) == '-')
			{
				s++;
				while (*s && *s != c)
					s++;
			}
		}
	}
	result[i] = NULL;
	return (result);
}