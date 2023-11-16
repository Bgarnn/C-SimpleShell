/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mns3-main_while.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kburalek <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 15:52:13 by kburalek          #+#    #+#             */
/*   Updated: 2023/11/16 15:52:15 by kburalek         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static char	*input_msg_init(void)
{
	char	*input_msg;
	char	path_dir[PATH_MAX];

	getcwd(path_dir, sizeof(path_dir));
	input_msg = strjoin_f(strjoin_f (ft_strdup("\x1b[34m"), \
		ft_strdup(path_dir)), ft_strdup("$\x1b[0m "));
	if (!input_msg)
		input_msg = ft_strdup("minishell_input: ");
	return (input_msg);
}

int	main_while(t_data *data)
{
	char	*input;
	char	*input_msg;

	input_msg = input_msg_init();
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
