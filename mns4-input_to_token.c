/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mns4-input_to_token.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kburalek <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 15:52:23 by kburalek          #+#    #+#             */
/*   Updated: 2023/11/16 15:52:25 by kburalek         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	split_space(t_data *data, t_token_ptr *p_tmp, t_token_node *src_h)
{
	char	*p_run;
	char	*p_same;

	(void)(data);
	p_run = src_h->value;
	p_same = src_h->value;
	while (1)
	{
		p_same = match_outside_qoute(p_same, " ", 0);
		if (p_same - p_run)
			make_token(p_tmp, src_h->mark, ft_substr(p_run, 0, p_same - p_run));
		if (!*p_same)
			break ;
		p_same = p_same + 1;
		p_run = p_same;
	}
}

static void	split_quote(t_data *data, t_token_ptr *p_tmp, t_token_node *src_h)
{
	int		sq;
	int		dbq;
	char	*res;
	size_t	i;
	size_t	res_i;

	(void)(data);
	sq = 0;
	dbq = 0;
	i = 0;
	res_i = 0;
	res = ft_calloc(ft_strlen(src_h->value) + 1, 1);
	while (src_h->value[i])
	{
		if (!sq && src_h->value[i] == '\"')
			dbq = !dbq;
		else if (!dbq && src_h->value[i] == '\'')
			sq = !sq;
		else
			res[res_i++] = src_h->value[i];
		i++;
	}
	make_token(p_tmp, src_h->mark, res);
}

static void	split_tilde(t_data *data, t_token_ptr *p_tmp, t_token_node *src_h)
{
	int	len;

	if (!src_h || !src_h->value)
		return ;
	len = ft_strlen(src_h->value);
	if (len == 1 && (!ft_strncmp(src_h->value, "~", 1)))
		return (make_token(p_tmp, src_h->mark, ft_strdup(env_k(data, "HOME"))));
	make_token(p_tmp, src_h->mark, ft_strdup(src_h->value));
}

static void	token_split(t_data *data, t_token_ptr *src, t_token_ptr *dst, \
	void (*fn)(t_data *data, t_token_ptr *p_tmp, t_token_node *src_h))
{
	t_token_ptr		p_tmp;
	t_token_node	*head_ptr;

	while (src->head)
	{
		p_tmp.head = 0;
		p_tmp.tail = 0;
		if (!fn || !src || !dst || !src->head)
			return ;
		fn (data, &p_tmp, src->head);
		head_ptr = src->head;
		src->head = src->head->next;
		free(head_ptr->value);
		free(head_ptr);
		if (!p_tmp.head)
			return ;
		if (!dst->head)
			dst->head = p_tmp.head;
		else
			dst->tail->next = p_tmp.head;
		dst->tail = p_tmp.tail;
	}
}

void	input_to_token(t_data *data, char *input)
{
	t_token_ptr	tmp1;
	t_token_ptr	tmp2;
	t_token_ptr	*output;

	output = &data->lst_token;
	if (!input || !output)
		return ;
	tmp1.head = 0;
	tmp1.tail = 0;
	tmp2.head = 0;
	tmp2.tail = 0;
	make_token(&tmp1, m_undefined, input);
	token_split (data, &tmp1, &tmp2, split_space);
	token_split (data, &tmp2, &tmp1, split_heredoc);
	token_split (data, &tmp1, &tmp2, split_out_app);
	token_split (data, &tmp2, &tmp1, split_out_tr);
	token_split (data, &tmp1, &tmp2, split_infile);
	token_split (data, &tmp2, &tmp1, split_pipe);
	token_split (data, &tmp1, &tmp2, split_dollar0);
	token_split (data, &tmp2, &tmp1, split_dollar1);
	token_split (data, &tmp1, &tmp2, split_tilde);
	token_split (data, &tmp2, &tmp1, split_quote);
	data->lst_token.head = tmp1.head;
	data->lst_token.tail = tmp1.tail;
}
