/*
gcc -I/opt/homebrew/Cellar/readline/8.2.1/include -L/opt/homebrew/Cellar/readline/8.2.1/lib -o mns mns.c libft/libft.a -lreadline -lncurses
gcc -I/usr/local/Cellar/readline/8.2.1/include -L/usr/local/Cellar/readline/8.2.1/lib -o mns mns.c libft/libft.a -lreadline -lncurses
*/

/////////////////////////////////* * PLAN * *////////////////////////////////////
/*
	- readline
	- signal
	- unorganized_token (except error , $, ~)
	- organized_token <-- NOW
	- err_check
	- grouped_token
*/
/////////////////////////////////////////////////////////////////////////////////

#include "minishell.h"

////////////////////////////**DEBUG FUNCTION**////////////////////////////////////

// print token_node 
void print_token_node(t_token_node *node) 
{
	if (node) 
	{
		printf("Mark: %u\n", node->mark); // show in index
		printf("Type: %s\n", node->type); 
		printf("Value: %s\n", node->value); 
	} 
	else
		printf("Node is NULL\n");
}

// print link list
void print_link_list(t_token_node *head) 
{
	int i = 1;
	t_token_node *current = head;
	while (current != NULL)
	{
		printf("\nnumber: %d\n", i);
		print_token_node(current);
		printf("\n");
		current = current->next;
		i++;
	}
	printf("\nNULL\n");
}

////////////////////////////mns_free.c////////////////////////////////////

void	free_mns(t_data *data)
{
	rl_clear_history();
}

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


/////////////////////////////////////////////////////////////////////////////////

t_token_node	*token_node_create (t_token_ptr *ptr, t_token_mark mark)
{
	t_token_node	*new;

	if(!ptr)
		return (NULL);
	new = (t_token_node *)malloc(sizeof(t_token_node) * 1);
	if (!new)
		return (NULL);
	new->mark = mark;
	new->type = 0;
	new->value = 0;
	new->next = 0;
	if (ptr->head)
	{
		(ptr->tail)->next = new;
		ptr->tail = (ptr->tail)->next;
		return (new);
	}
	else
	{
		ptr->head = new;
		ptr->tail = ptr->head;
		return (new);
	}
}

/////////////////////////////token_to_organize_utils.c///////////////////////////////////


void	group_init (t_list_ptr	*group)
{
	group->infile.head = 0;
	group->infile.tail = 0;
	group->outfile.head = 0;
	group->outfile.tail = 0;
	group->cmd.head = 0;
	group->cmd.tail = 0;
}

void	skip_next_node(t_token_node *node)
{
	t_token_node	*tmp;

	if(!node->next)
		return ;
	tmp = node->next;
	node->next = node->next->next;
	free(tmp->value);
	free(tmp);
}
///////////////////////////////token_to_organize.c//////////////////////////////////////

	// t_token_node *dst_tail;
	
	// if (!src || !src->head)
	// 	return ;
	// if (!dst)
	// 	dst->next = src->head
	// if (dst)
	// {
	// 	dst_tail = dst;
	// 	while (dst_tail->next)
	// 		dst_tail = dst_tail->next;
	// 	dst_tail->next = sec->head;
	// }

	
void	move_token(t_token_ptr *src, t_token_ptr *dst)
{
	// if (!dst || !src || !src->head)
	// 	return ;
	if (!dst->head)
	{
		dst->head = src->head;
		dst->tail = src->tail;
	}
	else
	{
		dst->tail->next = src->head;
		dst->tail = src->tail;
	}
}

void	tmp_group_to_organize(t_list_ptr *group, t_token_ptr *output, int pipe)
{
	//in each of tmp group is link list
	//move_token: is to move from one link llist(in tmp group) to another link list(output)


	// **group(input) : CORRECT**
	printf("\nGROUP INPUT\n");
	printf("\nCMD output:\n");// ** DEBUG **********************************************************************
	print_link_list(group->cmd.head); // ** DEBUG **********************************************************************

	printf("\nINFILE output:\n");// ** DEBUG **********************************************************************
	print_link_list(group->infile.head); // ** DEBUG **********************************************************************
	
		printf("\nOUTFILE output:\n");// ** DEBUG **********************************************************************
	print_link_list(group->outfile.head); // ** DEBUG **********************************************************************


	if(group->cmd.head)
		move_token(&group->cmd, output);
	if(group->infile.head)
		move_token(&group->infile, output);
	// print_link_list(output->head); // ** DEBUG **********************************************************************
	// printf("infile:\n");// ** DEBUG **********************************************************************
	// print_link_list(group->infile.head); // ** DEBUG **********************************************************************
	// printf("\nHi\n");// ** DEBUG **********************************************************************
	
	if(group->outfile.head)
		move_token(&group->outfile, output);


	if (pipe)
		token_node_create(output, m_pipe)->value = ft_strdup("|");
	// if (!group->cmd.head->value)
	// {
	// 	output->head = 
	// }
	group_init (group);
}

void	unorganize_to_tmp_group(t_list_ptr *group, t_token_ptr *output, t_token_node *src_head)
{
	// ** input: CORRECT **

	// printf("\nun_to_tmp_g input:\n"); // ** DEBUG ********************************************************************
	// print_token_node(src_head); // ** DEBUG ********************************************************************

	if (src_head->mark == m_undefined)
		token_node_create(&group->cmd, m_cmd)->value = ft_strdup(src_head->value);
	if (src_head->next)
	{
		// ** input: CORRECT **

		// printf("\ninput:\n"); // ** DEBUG ********************************************************************
		// print_token_node(src_head); // ** DEBUG ********************************************************************

		if (src_head->mark == m_heredoc || src_head->mark == m_infile)
		{
			token_node_create(&group->infile, src_head->mark)->value = ft_strdup(src_head->next->value);
			skip_next_node(src_head);
		}
	
		if (src_head->mark == m_out_append|| src_head->mark == m_out_trunc)
		{
			token_node_create(&group->outfile, src_head->mark)->value = ft_strdup(src_head->next->value);
			skip_next_node(src_head);
		}

		// printf("\nOUTFILE output:\n");// ** DEBUG **********************************************************************
		// print_link_list(group->outfile.head); // ** DEBUG **********************************************************************
	}
	if (!src_head->next)
		return (tmp_group_to_organize(group, output, 0));
		// tmp_group_to_organize(group, output, 0);
		// return ;
	
	if (src_head->mark == m_pipe)
		return (tmp_group_to_organize(group, output, 0));

	// **group : CORRECT**
	// printf("\nCMD output:\n");// ** DEBUG **********************************************************************
	// print_link_list(group->cmd.head); // ** DEBUG **********************************************************************

	// printf("\nINFILE output:\n");// ** DEBUG **********************************************************************
	// print_link_list(group->infile.head); // ** DEBUG **********************************************************************
	
	// 	printf("\nOUTFILE output:\n");// ** DEBUG **********************************************************************
	// print_link_list(group->outfile.head); // ** DEBUG **********************************************************************

}

void	unorganize_to_organize(t_list_ptr *group, t_token_ptr *src, t_token_ptr *dst)
{
	t_token_ptr		tmp_ptr;
	t_token_node	*head_ptr;
	
	// ** input: CORRECT **

	// printf("\nun_to_or input:\n"); // ** DEBUG ********************************************************************
	// print_link_list(src->head); // ** DEBUG ********************************************************************

	tmp_ptr.head = 0;
	tmp_ptr.tail = 0;
	if(!src || !dst || !src->head)
		return ;
	unorganize_to_tmp_group(group, &tmp_ptr, src->head);
	head_ptr = src->head;
	src->head = src->head->next;
	free(head_ptr->value);
	free(head_ptr);
	if(!tmp_ptr.head)
		return ;
	if (!dst->head)
		dst->head = tmp_ptr.head;
	else
		dst->tail->next = tmp_ptr.tail;
	dst->tail = tmp_ptr.tail;

		
	// ** dst: NOT CORRECT **

	// printf("\noutput:\n"); // ** DEBUG ********************************************************************
	// print_link_list(dst->head); // ** DEBUG ******************************************************************

}

void	token_to_organize(t_data *data, t_token_ptr *input)
{
	t_list_ptr	tmp_group;
	// t_token_ptr	*success_group_token;
	// success_group_token = &data->grouped_token;

	// ** input: CORRECT **

	// printf("\nt_to_o input:\n"); // ** DEBUG ********************************************************************
	// print_link_list(input->head); // ** DEBUG ********************************************************************

	int i = 1;// ** DEBUG ********************************************************************
	group_init (&tmp_group);
	while (input->head)
	{
		// ** input loop: CORRECT **

		printf("\nwhile loop %d\n", i); // * DEBUG ********************************************************************
		// print_link_list(input->head); // ** DEBUG ********************************************************************
		i++;// ** DEBUG ********************************************************************

		if(!input->head->value)
		{
			free_token_list(tmp_group.infile.head);
			free_token_list(tmp_group.outfile.head);
			free_token_list(tmp_group.cmd.head);
			free_token_list(input->head);
			free_token_list(data->organized_token.head);
			return ;
		}
		unorganize_to_organize(&tmp_group, input, &data->organized_token);
	}
	
	// ** data->organized_token): NOT CORRECT **

	// printf("\noutput:\n"); // ** DEBUG ********************************************************************
	// print_link_list(data->organized_token.head); // ** DEBUG ******************************************************************


	// err_check(data);
	// ungroup_to_group(data);
	// if (!success_group_token)
	// {
	// 	free_mns(data);
	// 	exit (0);
	// }
}



////////////////////////////input_to_token_utils2.c////////////////////////////////////

char	*match_outside_qoute(char *tmp_ptr, char *must_match, int match_in_dbq)
{
	int		sq;
	int		dbq;

	sq = 0;
	dbq = 0;
	while (*tmp_ptr)
	{
		if (!dbq && *tmp_ptr == '\'')
			sq = !sq;
		if (!sq && *tmp_ptr == '\"')
			dbq = !dbq;
		if (!ft_strncmp(tmp_ptr, must_match, ft_strlen(must_match)) && !sq && (!dbq || match_in_dbq))
			break ;
		tmp_ptr++;
	}
	return (tmp_ptr);
}

void	split_quote(t_data *data, t_token_ptr *tmp_ptr, t_token_node *src_head)
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
	res = ft_calloc(ft_strlen(src_head->value) + 1, 1);
	while (src_head->value[i])
	{
		if (!sq && src_head->value[i] == '\"')
			dbq = !dbq;
		else if (!dbq && src_head->value[i] == '\'')
			sq = !sq;
		else
			res[res_i++] = src_head->value[i];
		i++;
	}
	token_node_create(tmp_ptr, src_head->mark)->value = res;
}

void	split_pipe(t_data *data, t_token_ptr *tmp_ptr, t_token_node *src_head)
{
	char	*main_cptr;
	char	*match_cptr;
	size_t	len;

	(void)(data);
	main_cptr = src_head->value;
	match_cptr = src_head->value;
	while (1)
	{
		match_cptr = match_outside_qoute(match_cptr, "|", 0);
		len = match_cptr - main_cptr;
		if (len)
			token_node_create(tmp_ptr, src_head->mark)->value = ft_substr(main_cptr, 0, len);
		if (*match_cptr)
			token_node_create(tmp_ptr, m_pipe)->value = ft_strdup("|");
		else
			break ;
		match_cptr += 1;
		main_cptr = match_cptr;
	}
}

void	split_infile(t_data *data, t_token_ptr *tmp_ptr, t_token_node *src_head)
{
	char	*main_cptr;
	char	*match_cptr;
	size_t	len;

	(void)(data);
	main_cptr = src_head->value;
	match_cptr = src_head->value;
	if (src_head->mark == m_heredoc)
	{
		token_node_create(tmp_ptr, src_head->mark)->value = ft_strdup("<<");
		return ;
	}
	while (1)
	{
		match_cptr = match_outside_qoute(match_cptr, "<", 0);
		len = match_cptr - main_cptr;
		if (len)
			token_node_create(tmp_ptr, src_head->mark)->value = ft_substr(main_cptr, 0, len);
		if (*match_cptr)
			token_node_create(tmp_ptr, m_infile)->value = ft_strdup("<");
		else
			break ;
		match_cptr += 1;
		main_cptr = match_cptr;
	}
}

////////////////////////////input_to_token_utils1.c////////////////////////////////////

static void	split_out_trunc(t_data *data, t_token_ptr *tmp_ptr, t_token_node *src_head)
{
	char	*main_cptr;
	char	*match_cptr;
	size_t	len;

	(void)(data);
	main_cptr = src_head->value;
	match_cptr = src_head->value;
	if (src_head->mark == m_out_append)
	{
		token_node_create(tmp_ptr, src_head->mark)->value = ft_strdup(">>");
		return ;
	}
	while (1)
	{
		match_cptr = match_outside_qoute(match_cptr, ">", 0);
		len = match_cptr - main_cptr;
		if (len)
			token_node_create(tmp_ptr, src_head->mark)->value = ft_substr(main_cptr, 0, len);
		if (*match_cptr)
			token_node_create(tmp_ptr, m_out_trunc)->value = ft_strdup(">");
		else
			break ;
		match_cptr += 1;
		main_cptr = match_cptr;
	}
}

static void	split_out_append(t_data *data, t_token_ptr *tmp_ptr, t_token_node *src_head)
{
	char	*main_cptr;
	char	*match_cptr;
	size_t	len;

	(void)(data);
	main_cptr = src_head->value;
	match_cptr = src_head->value;
	while (1)
	{
		match_cptr = match_outside_qoute(match_cptr, ">>", 0);
		len = match_cptr - main_cptr;
		if (len)
			token_node_create(tmp_ptr, src_head->mark)->value = ft_substr(main_cptr, 0, len);
		if (*match_cptr)
			token_node_create(tmp_ptr, m_out_append)->value = ft_strdup(">>");
		else
			break ;
		match_cptr += 2;
		main_cptr = match_cptr;
	}
}

static void	split_heredoc(t_data *data, t_token_ptr *tmp_ptr, t_token_node *src_head)
{
	char	*main_cptr;
	char	*match_cptr;
	size_t	len;

	(void)(data);
	main_cptr = src_head->value;
	match_cptr = src_head->value;
	while (1)
	{
		match_cptr = match_outside_qoute(match_cptr, "<<", 0);
		len = match_cptr - main_cptr;
		if (len)
			token_node_create(tmp_ptr, src_head->mark)->value = ft_substr(main_cptr, 0, len);
		if (*match_cptr)
			token_node_create(tmp_ptr, m_heredoc)->value = ft_strdup("<<");
		else
			break ;
		match_cptr += 2;
		main_cptr = match_cptr;
	}
}

static void	split_space(t_data *data, t_token_ptr *tmp_ptr, t_token_node *src_head)
{
	char	*main_cptr;
	char	*match_cptr;
	size_t	len;

	(void)(data);
	main_cptr = src_head->value;
	match_cptr = src_head->value;
	while (1)
	{
		match_cptr = match_outside_qoute(match_cptr, " ", 0);
		len = match_cptr - main_cptr;
		if (len)
			token_node_create(tmp_ptr, src_head->mark)->value = ft_substr(main_cptr, 0, len);
		if (!*match_cptr)
			break ;
		match_cptr += 1;
		main_cptr = match_cptr;
	}
}

void	token_split(t_data *data, t_token_ptr *src, t_token_ptr *dst, void (*fn)(t_data *data, t_token_ptr *tmp_ptr, t_token_node *src_head))
{
	t_token_ptr		tmp_ptr;
	t_token_node	*head_ptr;

	tmp_ptr.head = 0;
	tmp_ptr.tail = 0;

	while (src->head)
	{
		if(!fn || !src || !dst || !src->head)
			return ;
		fn (data, &tmp_ptr, src->head);
		head_ptr = src->head;
		src->head = src->head->next;
		free(head_ptr->value);
		free(head_ptr);
		if(!tmp_ptr.head)
			return ;
		if (!dst->head)
			dst->head = tmp_ptr.head;
		else
			dst->tail->next = tmp_ptr.tail;
		dst->tail = tmp_ptr.tail;
	}
}

////////////////////////////input_to_token.c////////////////////////////////////

void	input_to_token(t_data *data, char *input)
{
	t_token_ptr	tmp1;
	t_token_ptr	tmp2;
	t_token_ptr	*output;

	output = &data->unorganized_token;
	if(!input || !output)
		return ;
	tmp1.head = 0;
	tmp1.tail = 0;
	tmp2.head = 0;
	tmp2.tail = 0;
	token_node_create(&tmp1, m_undefined)->value = input;
	// free(input); //ไม่แน่ใจว่าต้องฟรีมั้ย แต่เอาค่าไป = valueแล้ว ซึ่งจะฟรีvalueทีหลัง
	token_split (data, &tmp1, &tmp2, split_space);
	token_split (data, &tmp2, &tmp1, split_heredoc);
	token_split (data, &tmp1, &tmp2, split_out_append);
	token_split (data, &tmp2, &tmp1, split_out_trunc);
	token_split (data, &tmp1, &tmp2, split_infile);
	token_split (data, &tmp2, &tmp1, split_pipe);
	// token_split (data, &tmp1, &tmp2, split_dollar_sign); ** not finish **
	// token_split (data, &tmp2, &tmp1, split_tilde_symbol); ** not finish **
	token_split (data, &tmp1, &tmp2, split_quote);
	data->unorganized_token.head = tmp2.head;
	data->unorganized_token.tail = tmp2.tail;

	// print_link_list(data->unorganized_token.head); // ** DEBUG **********************************************************************
}

////////////////////////////mns_init.c////////////////////////////////////

static void	env_init(t_data *data, char **envp)
{
	int	i;
	int	row;

	i = 0;
	row = 0;
	while (envp[row])
		row++;
	data->env = (char **)malloc(sizeof(char *) * (row + 1));
	if(!data->env)
		return ;
	while (i < row)
	{
		data->env[i] = ft_strdup(envp[i]);
		if(!data->env[i])
		{
			free_char_2d(data->env);
			return ;
		}
		i++;
	}
	data->env[row] = 0;
	data->env_row_max = row;
}

static void	sig_quit_handling(int sig_num)
{
	if (sig_num == SIGINT && g_signal == 0)
		rl_redisplay();
}

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

static void	signal_init (t_data *data)
{
	t_sigaction	sig_int;
	t_sigaction	sig_quit;

	sig_int.sa_handler = sig_int_handling;
	sigemptyset(&sig_int.sa_mask);
	sig_int.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sig_int, NULL);
	sig_quit.sa_handler = sig_quit_handling;
	sigemptyset(&sig_quit.sa_mask);
	sig_quit.sa_flags = SA_RESTART;
	sigaction(SIGQUIT, &sig_quit, NULL);
}

void	mns_init (t_data *data, char **envp)
{
	g_signal = 0;
	data->errnum = 0;
	data->env = 0;
	data->env_row_max = 0;
	data->unorganized_token.head = 0;
	data->unorganized_token.tail = 0;
	data->organized_token.head = 0;
	data->unorganized_token.tail = 0;


	signal_init (data);
	env_init(data, envp);
}

////////////////////////////minishell.c////////////////////////////////////

char	*ft_strjoin_free(char *src, char *dst)
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
	while(src[j])
		res[i++] = src[j++];
	j = 0;
	while (dst[j])
		res[i++] = dst[j++];
	res[i] = '\0';
	return (free(src), free(dst), res);
}

static char	*input_msg_init (t_data *data)
{
	char	*path_color;
	char	*err_color;
	char	*input_msg;
	char	path_dir[PATH_MAX];

	getcwd(path_dir, sizeof(path_dir));
	path_color = ft_strjoin_free (ft_strdup("\x1b[34m"), ft_strdup(path_dir));
	err_color = ft_strjoin_free(ft_strdup("\x1b[33m?"), ft_itoa(data->errnum));
	input_msg = ft_strjoin_free(ft_strjoin_free(path_color, err_color), ft_strdup("\x1b[34m $\x1b[0m "));
	// input_msg = ft_strjoin_free(ft_strjoin_free (ft_strdup("\x1b[34m"), ft_strdup(path_dir)), ft_strdup("$\x1b[0m "));
	if (!input_msg)
		input_msg = ft_strdup("minishell_input: ");
	return(input_msg);
}

static int	main_while (t_data *data)
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
		free (input);
		return (0);
	}
	add_history(input);
	input_to_token(data, input);
	token_to_organize(data, &data->unorganized_token);
	return(0);

}

int main(int argc, char **argv, char **envp) 
{
	t_data	data;

	(void) argv;
	if (argc != 1)
		return (0);
	mns_init(&data, envp);
	while (1)
	{
		if(main_while(&data))
			break ;
		g_signal = 1;
		// excute_cmd(&data);
		g_signal = 0;
	}
	printf ("exit\n");
	free_mns(&data);
	return (0);
}

////////////////////////////**DEBUG**////////////////////////////////////

// print env_init
	// for (i = 0; i < row; i++) {
	// 	printf("data->env[%d] = %s\n", i, data->env[i]);}

// print input
	// if (input) {
	// 	printf("You entered: %s\n", input);
	// 	free(input); 
	// }

// printf
	// printf("%s", "checked");