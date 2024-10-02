#include "libft.h"

void	ft_lst_insertattail(t_list **lst, t_list *newnode)
{
	t_list *start;

	start = *lst;
	if (*lst)
	{
		while (start->next)
			start = start->next;
		start->next = newnode;
	}
	else
		*lst = newnode;
}