/*
gcc -I/opt/homebrew/Cellar/readline/8.2.1/include -L/opt/homebrew/Cellar/readline/8.2.1/lib -o mns mns.c libft/libft.a -lreadline -lncurses
gcc -I/usr/local/Cellar/readline/8.2.1/include -L/usr/local/Cellar/readline/8.2.1/lib -o mns mns.c libft/libft.a -lreadline -lncurses
*/
/* ************************************************************************** */

/*
	- readline
	- signal
	- lst_token (except error , $, ~)
	- organized_token <-- NOW
	- err_check
	- grouped_token
*/
/* ************************************************************************** */

#include "minishell.h"

////////////////////////////**DEBUG FUNCTION**/////////////////////////////////

// // print token_node 
// void print_token_node(t_token_node *node) 
// {
// 	if (node) 
// 	{
// 		printf("Mark: %u\n", node->mark); // show in index
// 		printf("Value: %s\n", node->value); 
// 	} 
// 	else
// 		printf("Node is NULL\n");
// }

// // print link list (t_token_node)
// void print_link_list(t_token_node *head) 
// {
// 	int i = 1;
// 	t_token_node *current = head;
// 	while (current != NULL)
// 	{
// 		printf("\nnumber: %d\n", i);
// 		print_token_node(current);
// 		printf("\n");
// 		current = current->next;
// 		i++;
// 	}
// 	printf("\nNULL\n");
// }

// // print link list (t_list_node)
// void print_group_list(t_list_node *head) 
// {
// 	t_list_node *current = head;
// 	int i = 1;
// 	while (current != NULL) 
// 	{
// 		printf("\nnode number: %d\n", i);
// 		printf("is_pipe: %d\n", current->is_pipe);

// 		// Print infile
// 		t_token_node *infile = current->infile;
// 		printf("infile: ");
// 		while (infile != NULL) {
// 			printf("%s(%u) -> ", infile->value, infile->mark);
// 			infile = infile->next;
// 		}
// 		printf("NULL\n");

// 		// Print outfile
// 		t_token_node *outfile = current->outfile;
// 		printf("outfile: ");
// 		while (outfile != NULL) {
// 			printf("%s(%u) -> ", outfile->value, outfile->mark);
// 			outfile = outfile->next;
// 		}
// 		printf("NULL\n");

// 		// Print cmd
// 		t_token_node *cmd = current->cmd;
// 		printf("cmd: ");
// 		while (cmd != NULL) {
// 			printf("%s(%u) -> ", cmd->value, cmd->mark);
// 			cmd = cmd->next;
// 		}
// 		printf("NULL\n");
// 		i++;
// 		current = current->next;
// 	}
// }

/* ************************************************************************** */
//////////////////////////////PUT IN-->minishell.c//////////////////////////////

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
	if (!dst || !*dst)
		return (src);
	else if (!src || !*src)
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

/* ************************************************************************** */
//////////////////////////PUT IN-->mns1-mns_init.c//////////////////////////////

void	group_init(t_group_ptr	*group)
{
	group->infile.head = 0;
	group->infile.tail = 0;
	group->outfile.head = 0;
	group->outfile.tail = 0;
	group->cmd.head = 0;
	group->cmd.tail = 0;
}

/* ************************************************************************** */
/////////////////////////////mns8-mns_free.c////////////////////////////////////

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

/* ************************************************************************** */
/////////////////////////////mns7-token_to_organize.c///////////////////////////

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

/* ************************************************************************** */
//////////////////////////////mns6-input_to_token.c/////////////////////////////

static size_t	index_of_c(const char *s, int c)
{
	size_t	i;

	i = 0;
	while (s[i] && s[i] != c)
		i++;
	return (i);
}

static char	*env_k(t_data *data, char *key)
{
	size_t	i;
	size_t	str_len;
	size_t	key_len;
	char	**env;

	env = data->env;
	key_len = ft_strlen(key);
	i = 0;
	while (env[i])
	{
		str_len = index_of_c(env[i], '=');
		if (key_len > str_len)
			str_len = key_len;
		if (ft_strncmp(key, env[i], str_len) == 0)
			return (ft_strchr(env[i], '=') + 1);
		i++;
	}
	return (NULL);
}

static char	*epur_str(char *src, int flg)
{
	char	*dst;
	size_t	src_i;
	size_t	dst_i;

	src_i = 0;
	dst_i = 0;
	dst = (char *)calloc((ft_strlen(src) + 1),sizeof(char));
	if (!dst)
		return(NULL);
	while (src[src_i] == ' ' || src[src_i] == '\t')
		src_i++;
	while (src[src_i])
	{
		if (!(src[src_i] == ' ' || src[src_i] == '\t'))
		{
			if (flg)
				dst[dst_i++] = ' ';
			flg = 0;
			dst[dst_i++] = src[src_i];
		}
		else
			flg = 1;
		src_i++;
	}
	return (dst);
}

static void	ft_rpl(char **env_rpl, char **str, char **p_same, t_data *data)
{
	char	*env_c;
	char	*tmp;
	char	*get_env;
	char	*p_check;
	int		flg;

	flg = 0;
	p_check = (*p_same);
	env_c = ft_substr(*p_same, 1, *str - *p_same - 1);
	get_env = env_k(data, env_c);
	tmp = *env_rpl;
	if (get_env)
	{
		if (data->in_dbq == 0)
			*env_rpl = strjoin_f(tmp, epur_str(get_env, flg));
		else if (data->in_dbq == 1)
			*env_rpl = strjoin_f(tmp, ft_strdup(get_env));
	}
	free(env_c);
}

void	env_check(char **env_rpl, char **p_nonchar, char **p_same, t_data *data)
{
	char	*str;

	str = (*p_same) + 1;
	if (*str == '?')
	{
		str++;
		*env_rpl = strjoin_f((*env_rpl), ft_itoa(data->errnum));
		*p_nonchar = str;
		return ;
	}
	while (*str && (ft_isalnum(*str) || *str == '_'))
		str++;
	if (str > (*p_same) + 1)
		ft_rpl( env_rpl, &str, p_same, data);
	else
		*env_rpl = strjoin_f(*env_rpl, ft_strdup("$"));
	*p_nonchar = str;
}

/* ************************************************************************** */
//////////////////////////////mns5-input_to_token.c/////////////////////////////

char	*match_outside_qoute(char *p_tmp, char *must_match, int match_in_dbq)
{
	int		sq;
	int		dbq;

	sq = 0;
	dbq = 0;
	while (*p_tmp)
	{
		if (!dbq && *p_tmp == '\'')
			sq = !sq;
		if (!sq && *p_tmp == '\"')
			dbq = !dbq;
		if (!ft_strncmp(p_tmp, must_match, ft_strlen(must_match)) \
				&& !sq && (!dbq || match_in_dbq))
			break ;
		p_tmp++;
	}
	return (p_tmp);
}

void	split_dollar1(t_data *data, t_token_ptr *dst_ptr, t_token_node *src_h)
{
	char	*p_run;
	char	*p_same;
	char	*p_nonchar;
	char	*env_rpl;

	env_rpl = 0;
	data->in_dbq = 1;
	p_run = src_h->value;
	p_same = src_h->value;
	while (1)
	{
		p_same = match_outside_qoute(p_same, "$", 1);
		if (p_same - p_run)
			env_rpl = strjoin_f(env_rpl, ft_substr(p_run, 0, p_same - p_run));
		if (*p_same)
			env_check(&env_rpl, &p_nonchar, &p_same, data);
		else
			break ;
		p_same = p_nonchar;
		p_run = p_same;
	}
	make_token(dst_ptr, src_h->mark, env_rpl);
}

void	split_dollar0(t_data *data, t_token_ptr *dst_ptr, t_token_node *src_h)
{
	char	*p_run;
	char	*p_same;
	char	*p_nonchar;
	char	*env_rpl;

	env_rpl = 0;
	data->in_dbq = 0;
	p_run = src_h->value;
	p_same = src_h->value;
	while (1)
	{
		p_same = match_outside_qoute(p_same, "$", 0);
		if (p_same - p_run)
			env_rpl = strjoin_f(env_rpl, ft_substr(p_run, 0, p_same - p_run));
		if (*p_same)
			env_check(&env_rpl, &p_nonchar, &p_same, data);
		else
			break ;
		p_same = p_nonchar;
		p_run = p_same;
	}
	make_token(dst_ptr, src_h->mark, env_rpl);
}

/* ************************************************************************** */
//////////////////////////////mns4-input_to_token.c/////////////////////////////

void	split_pipe(t_data *data, t_token_ptr *p_tmp, t_token_node *src_h)
{
	char	*p_run;
	char	*p_same;
	size_t	len;

	(void)(data);
	p_run = src_h->value;
	p_same = src_h->value;
	while (1)
	{
		p_same = match_outside_qoute(p_same, "|", 0);
		len = p_same - p_run;
		if (len)
			make_token(p_tmp, src_h->mark, ft_substr(p_run, 0, len));
		if (*p_same)
			make_token(p_tmp, m_pipe, ft_strdup("|"));
		else
			break ;
		p_same = p_same + 1;
		p_run = p_same;
	}
}

void	split_infile(t_data *data, t_token_ptr *p_tmp, t_token_node *src_h)
{
	char	*p_run;
	char	*p_same;
	size_t	len;

	(void)(data);
	p_run = src_h->value;
	p_same = src_h->value;
	if (src_h->mark == m_heredoc)
	{
		make_token(p_tmp, src_h->mark, ft_strdup("<<"));
		return ;
	}
	while (1)
	{
		p_same = match_outside_qoute(p_same, "<", 0);
		len = p_same - p_run;
		if (len)
			make_token(p_tmp, src_h->mark, ft_substr(p_run, 0, len));
		if (*p_same)
			make_token(p_tmp, m_infile, ft_strdup("<"));
		else
			break ;
		p_same = p_same + 1;
		p_run = p_same;
	}
}

static void	split_out_tr(t_data *data, t_token_ptr *p_tmp, t_token_node *src_h)
{
	char	*p_run;
	char	*p_same;

	(void)(data);
	p_run = src_h->value;
	p_same = src_h->value;
	if (src_h->mark == m_out_append)
	{
		make_token(p_tmp, src_h->mark, ft_strdup(">>"));
		return ;
	}
	while (1)
	{
		p_same = match_outside_qoute(p_same, ">", 0);
		if (p_same - p_run)
			make_token(p_tmp, src_h->mark, ft_substr(p_run, 0, p_same - p_run));
		if (*p_same)
			make_token(p_tmp, m_out_trunc, ft_strdup(">"));
		else
			break ;
		p_same = p_same + 1;
		p_run = p_same;
	}
}

void	split_out_app(t_data *data, t_token_ptr *p_tmp, t_token_node *src_h)
{
	char	*p_run;
	char	*p_same;

	(void)(data);
	p_run = src_h->value;
	p_same = src_h->value;
	while (1)
	{
		p_same = match_outside_qoute(p_same, ">>", 0);
		if (p_same - p_run)
			make_token(p_tmp, src_h->mark, ft_substr(p_run, 0, p_same - p_run));
		if (*p_same)
			make_token(p_tmp, m_out_append, ft_strdup(">>"));
		else
			break ;
		p_same = p_same + 2;
		p_run = p_same;
	}
}

void	split_heredoc(t_data *data, t_token_ptr *p_tmp, t_token_node *src_h)
{
	char	*p_run;
	char	*p_same;

	(void)(data);
	p_run = src_h->value;
	p_same = src_h->value;
	while (1)
	{
		p_same = match_outside_qoute(p_same, "<<", 0);
		if (p_same - p_run)
			make_token(p_tmp, src_h->mark, ft_substr(p_run, 0, p_same - p_run));
		if (*p_same)
			make_token(p_tmp, m_heredoc, ft_strdup("<<"));
		else
			break ;
		p_same = p_same + 2;
		p_run = p_same;
	}
}

/* ************************************************************************** */
////////////////////////////mns3-input_to_token.c//////////////////////////////

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

static void	token_split(t_data *data, t_token_ptr *src, t_token_ptr *dst, 
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

////////////////////////////mns2-main_while.c////////////////////////////////////
/* ************************************************************************** */

static void	err_print(t_data *data, char *str, int err_num)
{
	printf("%s", str);
	data->errnum = err_num;
}

static int	sign(t_token_node *src_h)
{
	int	len;

	if (!src_h || !src_h->value)
		return (0);
	len = ft_strlen(src_h->value);
	if (src_h->mark == m_pipe)
		return (1);
	if (src_h->mark == m_out_append || src_h->mark == m_out_trunc)
		return (2);
	if (src_h->mark == m_infile || src_h->mark == m_heredoc)
		return (2);
	if (ft_strnstr(src_h->value, "<<<", len))
		return (3);
	if (ft_strnstr(src_h->value, ">>>", len))
		return (3);
	if (ft_strnstr(src_h->value, "&", len))
		return (4);
	if (ft_strnstr(src_h->value, ";", len))
		return (4);
	return (0);
}

static int	err_check(t_data *data)
{
	t_token_node	*p_run;
	t_token_ptr		input;

	input = data->lst_token;
	p_run = input.head;
	if (!input.head)
		return (0);
	if (sign(input.head) == 1 || sign(input.tail) == 2 || sign(input.tail) == 3)
		return (err_print(data, "minishell: syntax error\n", 258), 1);
	if (sign(input.tail) == 4)
		return (err_print(data, "minishell: input error\n", 1), 1);
	while (p_run->next)
	{
		if (sign(p_run) == 3 || (sign(p_run) == 1 && sign(p_run->next) == 1) \
				|| (sign(p_run) == 2 && sign(p_run->next) == 1) \
				|| (sign(p_run) == 2 && sign(p_run->next) == 2))
			return (err_print(data, "minishell: syntax error\n", 258), 1);
		if (sign(p_run) == 4)
			return (err_print(data, "minishell: input error\n", 1), 1);
		p_run = p_run->next;
	}
	return (0);
}

static char	*input_msg_init(t_data *data)
{
	char	*path_color;
	char	*err_color;
	char	*input_msg;
	char	path_dir[PATH_MAX];

	getcwd(path_dir, sizeof(path_dir));
	path_color = strjoin_f (ft_strdup("\x1b[34m"), ft_strdup(path_dir));
	err_color = strjoin_f(ft_strdup("\x1b[33m?"), ft_itoa(data->errnum));
	input_msg = strjoin_f(strjoin_f(path_color, err_color), ft_strdup("\x1b[34m $\x1b[0m "));
	// input_msg = strjoin_f(strjoin_f (ft_strdup("\x1b[34m"), ft_strdup(path_dir)), ft_strdup("$\x1b[0m "));
	if (!input_msg)
		input_msg = ft_strdup("minishell_input: ");
	return (input_msg);
}

int	main_while (t_data *data)
{
	char	*input;
	char	*input_msg;

	input_msg = input_msg_init(data);
	input = readline(input_msg);
	free(input_msg);
	if (!input)
		return (1);
	if (!input[0])
	{
		free(input);
		data->errnum = 0;
		return (0);
	}
	add_history(input);
	input_to_token(data, input);
	if (err_check(data))
		return (0);
	token_to_organize(data, &data->lst_token);
	g_signal = 1;
	first_execute(data);
	return (0);
}

/* ************************************************************************** */
////////////////////////////mns1-mns_init.c////////////////////////////////////

static void	env_init(t_data *data, char **envp)
{
	int	i;
	int	row;

	i = 0;
	row = 0;
	while (envp[row])
		row++;
	data->env = (char **)malloc(sizeof(char *) * (row + 1));
	if (!data->env)
		return ;
	while (i < row)
	{
		data->env[i] = ft_strdup(envp[i]);
		if (!data->env[i])
			return (free_char_2d(data->env));
		i++;
	}
	data->env[row] = 0;
	data->env_row_max = row;
}

// static void	sig_quit_handling(int sig_num)
// {
// 	if (sig_num == SIGINT && g_signal == 0)
// 		rl_redisplay();
// }

static void	sig_int_handling(int sig_num)
{
	if (sig_num == SIGINT && g_signal == 0)
	{
		ft_putchar_fd('\n', STDOUT_FILENO);
		rl_on_new_line();
		rl_replace_line("", STDOUT_FILENO);
		rl_redisplay();
	}
}

static void	signal_init(void)
{
	t_sigaction	sig_int;
	t_sigaction	sig_quit;

	sig_int.sa_handler = sig_int_handling;
	sigemptyset(&sig_int.sa_mask);
	sig_int.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sig_int, NULL);
	// sig_quit.sa_handler = sig_quit_handling;
	sig_quit.sa_handler = SIG_IGN;
	sigemptyset(&sig_quit.sa_mask);
	sig_quit.sa_flags = SA_RESTART;
	sigaction(SIGQUIT, &sig_quit, NULL);
}

void	mns_init(t_data *data, char **envp)
{
	g_signal = 0;
	data->errnum = 0;
	data->in_dbq = 0;
	data->env = 0;
	data->env_row_max = 0;
	data->lst_token.head = 0;
	data->lst_token.tail = 0;
	data->grouped_token = 0;
	data->path_exec = 0;
	data->stdin_copy = 0;
	data->stdout_copy = 0;
	data->pid = 0;
	data->fd_in = 0;
	data->fd_out = 0;
	data->num_child = 0;
	data->index = 0;
	data->len_path = 0;
	data->builtin_parent = 0;
	signal_init ();
	env_init(data, envp);
}

/* ************************************************************************** */
////////////////////////////////minishell.c////////////////////////////////////

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

int main(int argc, char **argv, char **envp) 
{
	t_data	data;

	(void) argv;
	if (argc != 1)
	{
		printf("minishell: incorrect input");
		return (0);
	}
	mns_init(&data, envp);
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