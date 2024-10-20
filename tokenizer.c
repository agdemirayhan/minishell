/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aagdemir <aagdemir@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 13:50:59 by aagdemir          #+#    #+#             */
/*   Updated: 2024/10/20 13:51:16 by aagdemir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ttype	check_type(char *value)
{
	if (ft_strncmp(value, "|", 1) == 0)
		return (PIPE);
	else if (ft_strncmp(value, "<", 1) == 0 || ft_strncmp(value, ">", 1) == 0)
		return (REDIRECTION);
	else
		return (WORD);
}
