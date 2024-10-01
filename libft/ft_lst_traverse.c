#include "libft.h"

t_stack	*ft_lsttraverse(t_stack *stack)
{
	t_stack *last;

	if (!stack)
		return (NULL);
	while (stack)
	{
		if (!stack->next)
			last = stack;
		stack = stack->next;
	}
	return (last);
}