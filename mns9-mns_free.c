/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mns9-mns_free.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kburalek <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 15:53:16 by kburalek          #+#    #+#             */
/*   Updated: 2023/11/16 15:53:17 by kburalek         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_char_2d(char **ptr)
{
	int	i;

	i = 0;
	if (!ptr)
		return ;
	while (ptr[i])
	{
		free(ptr[i]);
		ptr[i] = NULL;
		i++;
	}
	free (ptr);
	ptr = NULL;
}

void	free_token_list(t_token_node *token_ptr)
{
	t_token_node	*tmp_node;

	while (token_ptr)
	{
		tmp_node = token_ptr->next;
		free(token_ptr->value);
		free(token_ptr);
		token_ptr = tmp_node;
	}
}

void	free_group_list(t_list_node *list_head)
{
	t_list_node	*tmp_head;

	while (list_head)
	{
		tmp_head = list_head;
		list_head = list_head->next;
		free_token_list(tmp_head->cmd);
		free_token_list(tmp_head->infile);
		free_token_list(tmp_head->outfile);
		free(tmp_head);
	}
}

void	free_mns(t_data *data)
{
	(void)(data);
	rl_clear_history();
	if (data->lst_token.head)
	{
		free_token_list(data->lst_token.head);
		data->lst_token.head = 0;
		data->lst_token.tail = 0;
	}
	if (data->grouped_token)
	{
		free_group_list(data->grouped_token);
		data->grouped_token = 0;
	}
	if (data->env)
		free_char_2d(data->env);
}
