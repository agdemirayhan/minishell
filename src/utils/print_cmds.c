/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_cmds.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aagdemir <aagdemir@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 21:56:13 by aagdemir          #+#    #+#             */
/*   Updated: 2024/10/21 21:56:15 by aagdemir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// void	print_cmds(t_list *cmds)
// {
// 	t_list	*current;
// 	t_mini	*cmd_data;
// 	int		cmd_index;

// 	cmd_index = 0;
// 	current = cmds;
// 	while (current != NULL)
// 	{
// 		cmd_data = (t_mini *)current->content;
// 		printf("Command %d:\n", cmd_index);
// 		printf("\tinfile: %d\n", cmd_data->infile);
// 		printf("\toutfile: %d\n", cmd_data->outfile);
// 		if (cmd_data->full_path)
// 			printf("\tfull_path: %s\n", cmd_data->full_path);
// 		else
// 			printf("\tfull_path: (builtin or not set)\n");
// 		if (cmd_data->full_cmd)
// 		{
// 			printf("\tfull_cmd: {");
// 			for (int i = 0; cmd_data->full_cmd[i] != NULL; i++)
// 			{
// 				printf("%s", cmd_data->full_cmd[i]);
// 				if (cmd_data->full_cmd[i + 1] != NULL)
// 					printf(", ");
// 			}
// 			printf("}\n");
// 		}
// 		else
// 			printf("\tfull_cmd: (none)\n");
// 		current = current->next;
// 		cmd_index++;
// 	}
// }
