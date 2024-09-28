#include "minishell.h"

void	print_cmds(t_stack *cmds)
{
	t_stack	*current;
	int		i;

	current = cmds;
	i = 0;
	while (current != NULL)
	{
		if (current->content == NULL)
		{
			printf("Warning: Node %d has NULL content\n", i);
		}
		else
		{
			printf("Node %d content: %s\n", i, (char *)current->content);
		}
		current = current->next;
		i++;
	}
}
