/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_memory.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plertsir <plertsir@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 09:48:33 by plertsir          #+#    #+#             */
/*   Updated: 2023/11/01 20:43:31 by plertsir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_everything(t_data *data)
{
	t_list_node		*curr_list;
	t_token_node	*curr_token;
	t_token_node	*tmp;
	// char			**curr_cmd;
	// // char	*cmd;		
	// int				i;

	tmp = NULL;
	curr_list = data->grouped_token;
	while (curr_list != NULL)
	{
		if (curr_list->infile != NULL)
		{
			curr_token = curr_list->infile;
			while (curr_token != NULL)
			{
				tmp = curr_token;
				if (curr_token->value != NULL)
					free(curr_token->value);
				curr_token = curr_token->next;
				free(tmp);
			}
		}
		if (curr_list->outfile != NULL)
		{
			curr_token = curr_list->outfile;
			while (curr_token != NULL)
			{
				tmp = curr_token;
				if (curr_token->value != NULL)
					free(curr_token->value);
				curr_token = curr_token->next;
				free(tmp);
			}
		}
		// if (curr_list->cmd != NULL)
		// {
		// 	i = 0;
		// 	curr_cmd = curr_list->cmd;
		// 	while (curr_cmd != NULL)
		// 	{
		// 		curr_cmd = *curr_cmd;
		// 		while (i < 2)
		// 		{
		// 			if (curr_cmd[i] != NULL)
		// 				free(curr_cmd[i]);
		// 			i++;
		// 		}
		// 	}
		// }
		curr_list = curr_list->next;
	}
	return ;
}
