#include "minishell.h"

static void	skip_next_node(t_token_node *node)
{
	t_token_node	*tmp;

	if (!node->next)
		return ;
	tmp = node->next;
	node->next = node->next->next;
	free(tmp->value);
	free(tmp);
}

static t_list_node	*make_list_node(t_group_ptr *group, int pipe)
{
	t_list_node	*new_node;

	if (!group || (!(group->cmd.head) && !(group->infile.head) \
			&& !(group->outfile.head)))
		return (NULL);
	new_node = (t_list_node *)malloc(sizeof(t_list_node) * 1);
	if (!new_node)
		return (NULL);
	new_node->is_pipe = 0;
	new_node->cmd = 0;
	new_node->infile = 0;
	new_node->outfile = 0;
	new_node->next = 0;
	if (group->cmd.head)
		new_node->cmd = group->cmd.head;
	if (group->infile.head)
		new_node->infile = group->infile.head;
	if (group->outfile.head)
		new_node->outfile = group->outfile.head;
	if (pipe)
		new_node->is_pipe = 1;
	group_init (group);
	return (new_node);
}

static t_list_node	*un_to_tmp_group(t_group_ptr *group, t_token_node *src_h)
{
	if (src_h->mark == m_undefined)
		make_token(&group->cmd, m_cmd, ft_strdup(src_h->value));
	if (src_h->next)
	{
		if (src_h->mark == m_heredoc || src_h->mark == m_infile)
		{
			make_token(&group->infile, src_h->mark, \
				ft_strdup(src_h->next->value));
			skip_next_node(src_h);
		}
		if (src_h->mark == m_out_append || src_h->mark == m_out_trunc)
		{
			make_token(&group->outfile, src_h->mark, \
				ft_strdup(src_h->next->value));
			skip_next_node(src_h);
		}
	}
	if (!src_h->next)
		return (make_list_node(group, 0));
	if (src_h->mark == m_pipe)
		return (make_list_node(group, 1));
	return (NULL);
}

static void	un_to_or(t_group_ptr *group, t_token_ptr *src, t_list_node *dst_h)
{
	t_list_node		*dst_t;
	t_list_node		*tmp_new;
	t_token_node	*head_ptr;

	if (!src || !src->head)
		return ;
	tmp_new = un_to_tmp_group(group, src->head);
	head_ptr = src->head;
	src->head = src->head->next;
	free(head_ptr->value);
	free(head_ptr);
	if (!tmp_new)
		return ;
	if (!dst_h)
		dst_h = tmp_new;
	else
	{
		dst_t = dst_h;
		while (dst_t->next)
			dst_t = dst_t->next;
		dst_t->next = tmp_new;
	}
}

void	token_to_organize(t_data *data, t_token_ptr *input)
{
	t_group_ptr	tmp_group;
	t_list_node	dst_h;

	dst_h.cmd = 0;
	dst_h.infile = 0;
	dst_h.is_pipe = 0;
	dst_h.outfile = 0;
	dst_h.next = 0;
	group_init (&tmp_group);
	while (input->head)
	{
		if (!input->head->value)
		{
			free_token_list(tmp_group.infile.head);
			free_token_list(tmp_group.outfile.head);
			free_token_list(tmp_group.cmd.head);
			free_token_list(input->head);
			return ;
		}
		un_to_or(&tmp_group, input, &dst_h);
	}
	data->grouped_token = dst_h.next;
}
