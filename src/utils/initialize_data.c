/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize_data.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aagdemir <aagdemir@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 21:55:09 by aagdemir          #+#    #+#             */
/*   Updated: 2024/10/21 21:55:26 by aagdemir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_data	*initialize_data(char **envp)
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	if (!data)
		exit(EXIT_FAILURE);
	data->shlvl_history = NULL;
	data->env_list = NULL;
	data->mini_count = 1;
	data->prev_exit_stat = 0;
	data->env_list = init_env_list(envp);
	if (!data->env_list)
	{
		cleanup(data);
		exit(EXIT_FAILURE);
	}
	return (data);
}

void	init_shell(t_data *data)
{
	signal(SIGINT, handle_signals);
	signal(SIGQUIT, SIG_IGN);
	data->mini_count = 1;
}
