#include "libft.h"

t_stack	*insert_at_tail(t_stack *stack, char *new)
{
	t_stack *new_node;
	t_stack *current;

	// Create a new node
	new_node = malloc(sizeof(t_stack));
	if (new_node == NULL)
		return (NULL);
	new_node->content = new;
	new_node->next = NULL;

	// If the list is empty, the new node becomes the head
	if (stack == NULL)
		return (new_node);

	// Otherwise, traverse to the end of the list and append the new node
	current = stack;
	while (current->next != NULL)
		current = current->next;
	current->next = new_node;
	return (stack); // Return the head of the list
}