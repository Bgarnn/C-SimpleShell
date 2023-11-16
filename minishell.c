/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kburalek <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 15:51:22 by kburalek          #+#    #+#             */
/*   Updated: 2023/11/16 15:51:27 by kburalek         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	make_token(t_token_ptr *ptr, t_token_mark mark, char *str)
{
	t_token_node	*new;

	if (!ptr)
		return ;
	new = (t_token_node *)malloc(sizeof(t_token_node) * 1);
	if (!new)
		return ;
	new->mark = mark;
	new->value = str;
	new->next = 0;
	if (ptr->head)
	{
		(ptr->tail)->next = new;
		ptr->tail = (ptr->tail)->next;
		return ;
	}
	else
	{
		ptr->head = new;
		ptr->tail = ptr->head;
		return ;
	}
}

char	*strjoin_f(char *src, char *dst)
{
	char	*res;
	size_t	count;
	size_t	i;
	size_t	j;

	if (!dst && !src)
		return (NULL);
	if (!dst)
		return (src);
	else if (!src)
		return (dst);
	count = ft_strlen(src) + ft_strlen(dst);
	res = (char *)malloc(sizeof(char) * (count + 1));
	if (!res)
		return (NULL);
	i = 0;
	j = 0;
	while (src[j])
		res[i++] = src[j++];
	j = 0;
	while (dst[j])
		res[i++] = dst[j++];
	res[i] = '\0';
	return (free(src), free(dst), res);
}

static void	free_cmd_history(t_data *data)
{
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
}

int	main(int argc, char **argv, char **envp)
{
	t_data	data;

	(void) argv;
	if (argc != 1)
	{
		printf("minishell: incorrect input");
		return (0);
	}
	if (!mns_init(&data, envp))
		return (0);
	while (1)
	{
		if (main_while(&data))
			break ;
		g_signal = 0;
		free_cmd_history(&data);
	}
	printf ("exit\n");
	free_mns(&data);
	return (0);
}
