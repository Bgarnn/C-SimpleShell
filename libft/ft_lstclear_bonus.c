/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plertsir <plertsir@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 16:01:13 by kburalek          #+#    #+#             */
/*   Updated: 2023/11/07 21:06:07 by plertsir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
