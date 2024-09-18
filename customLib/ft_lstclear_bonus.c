#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*currentnode;
	t_list	*run;

	if (lst == 0 || del == 0)
		return ;
	else if (lst != 0)
	{
		run = *lst;
		while (run != 0)
		{
			currentnode = run;
			run = run->next;
			del(currentnode->content);
			free(currentnode);
		}
		*lst = NULL;
	}
}
