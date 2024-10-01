#include "libft.h"

t_stack	*ft_lstnew(void *content)
{
	t_stack *s1;

	s1 = (t_stack *)malloc(sizeof(t_stack));
	if (!s1)
		return (NULL);
	s1->content = content;
	s1->next = NULL;
	return (s1);
}