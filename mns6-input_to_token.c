#include "minishell.h"

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

	p_nonchar = 0;
	data->in_dbq = 1;
	env_rpl = strdup("");
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

	p_nonchar = 0;
	data->in_dbq = 0;
	env_rpl = strdup("");
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
