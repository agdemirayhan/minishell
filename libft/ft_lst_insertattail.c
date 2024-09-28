#include "libft.h"

t_stack	*insert_at_tail(t_stack *stack, int new_content)
{
	t_stack *new_node;
	t_stack *current;

	new_node = malloc(sizeof(t_stack));
	if (new_node == NULL)
		return (NULL);
	new_node->content = new_content;
	new_node->next = NULL;
	if (stack == NULL)
		return (new_node);
	else
	{
		current = stack;
		while (current->next != NULL)
			current = current->next;
		current->next = new_node;
		return (stack);
	}
}