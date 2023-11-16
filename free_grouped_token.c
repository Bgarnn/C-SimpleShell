/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_grouped_token.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plertsir <plertsir@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 12:49:24 by plertsir          #+#    #+#             */
/*   Updated: 2023/11/15 14:38:47 by plertsir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_token_node(t_token_node *curr_token)
{
	t_token_node	*tmp;

	while (curr_token != NULL)
	{
		tmp = curr_token;
		if (curr_token->value != NULL)
		{
			free(curr_token->value);
			curr_token->value = NULL;
		}
		curr_token = curr_token->next;
		free(tmp);
		tmp = NULL;
	}
}

void	free_grouped_token(t_list_node *curr_list)
{
	t_list_node	*tmp;

	while (curr_list != NULL)
	{
		tmp = curr_list;
		if (curr_list->infile != NULL)
			free_token_node(curr_list->infile);
		if (curr_list->outfile != NULL)
			free_token_node(curr_list->outfile);
		if (curr_list->cmd != NULL)
			free_token_node(curr_list->cmd);
		curr_list = curr_list->next;
		free(tmp);
		tmp = NULL;
	}
}
