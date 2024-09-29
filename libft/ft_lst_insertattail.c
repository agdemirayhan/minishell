#include "libft.h"

t_stack	*insert_at_tail(t_stack *stack, void *new)
{
	t_stack	*new_node;
	t_stack	*last;

	// Allocate new node
	new_node = malloc(sizeof(t_stack));
	if (!new_node)
		return (NULL);
	// Set the content of the new node
	new_node->content = new;
	new_node->next = NULL;
	// If the list is empty, return the new node as the head
	if (!stack)
		return (new_node);
	// Traverse to the last node
	last = ft_lsttraverse(stack);
	last->next = new_node; // Add the new node at the end
	return (stack);
}
