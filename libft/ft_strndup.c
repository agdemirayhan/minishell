/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aagdemir <aagdemir@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 22:01:43 by aagdemir          #+#    #+#             */
/*   Updated: 2024/10/21 22:01:47 by aagdemir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strndup(const char *s1, size_t n)
{
	size_t len;
	char *str;

	len = ft_strlen(s1);
	if (len > n)
		len = n;
	str = malloc(sizeof(char) * (len + 1));
	if (!str)
		return (0);
	str = ft_memcpy(str, s1, len);
	str[len] = '\0';
	return (str);
}