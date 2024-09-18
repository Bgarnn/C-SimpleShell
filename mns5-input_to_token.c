#include "minishell.h"

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

void	split_out_tr(t_data *data, t_token_ptr *p_tmp, t_token_node *src_h)
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
