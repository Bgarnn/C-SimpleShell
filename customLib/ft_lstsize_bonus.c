#include "libft.h"

int	ft_lstsize(t_list *lst)
{
	t_list	*run;
	int		i;

	i = 0;
	run = lst;
	while (run != 0)
	{
		run = run->next;
		i++;
	}
	return (i);
}
// count node in (lst)