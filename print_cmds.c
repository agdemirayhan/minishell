#include "minishell.h"

void	print_cmds(t_stack *cmds)
{
	t_stack *current_node;
	t_mini *cmd_content;
	int i;
	int j = 0;

	current_node = cmds;
	if (current_node == NULL)
	{
		printf("CMD Node is NULL\n");
		return ;
	}
	while (current_node != NULL)
	{
		// Assuming content of t_stack is a t_mini structure
		cmd_content = (t_mini *)current_node->content;
		// Print the command (cmd_content->full_cmd) if it exists
		if (cmd_content && cmd_content->full_cmd)
		{
			printf("Command[%d]: ", j);
			i = 0;
			while (cmd_content->full_cmd[i])
			{
				printf("%s ", cmd_content->full_cmd[i]);
				// Print each part of the command
				i++;
			}
			printf("\n");
		}
		current_node = current_node->next;
		j++;
	}
}