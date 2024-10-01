#include "libft.h"

t_list	*ft_lsttraverse(t_list *stack)
{
	t_list *last;

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