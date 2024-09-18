#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*back;

	if (*lst == 0)
	{
		*lst = new;
	}
	else
	{
		back = ft_lstlast(*lst);
		back->next = new;
	}
}
