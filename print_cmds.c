#include "minishell.h"

/**
 * @brief Prints the contents of an array of linked lists.
 * Iterates over an array of linked lists `cmds` and
 * prints each command and argument in every list.
 * Each linked list represents a sequence of
 * commands/arguments, and multiple lists are divided by pipes.
 * @param cmds Array of linked lists,
 * each containing command nodes with string content.
 */
void	print_cmds(t_stack **cmds)
{
	t_stack	*current;
	int		list_index;
	int		node_index;

	list_index = 0;
	while (cmds[list_index])
	{
		printf("cmds[%d]: ", list_index);
		current = cmds[list_index];
		node_index = 0;
		while (current != NULL)
		{
			if (current->content == NULL)
				printf("(NULL)");
			else
				printf("%s", (char *)current->content);
			if (current->next)
				printf(" ");
			current = current->next;
			node_index++;
		}
		printf("\n");
		list_index++;
	}
}
