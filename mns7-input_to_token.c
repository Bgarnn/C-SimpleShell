/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mns7-input_to_token.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kburalek <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 15:52:55 by kburalek          #+#    #+#             */
/*   Updated: 2023/11/16 15:52:56 by kburalek         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	index_of_c(const char *s, int c)
{
	size_t	i;

	i = 0;
	while (s[i] && s[i] != c)
		i++;
	return (i);
}

char	*env_k(t_data *data, char *key)
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
	dst = (char *)calloc((ft_strlen(src) + 1), sizeof(char));
	if (!dst)
		return (NULL);
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
	int		flg;

	flg = 0;
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
		ft_rpl(env_rpl, &str, p_same, data);
	else
		*env_rpl = strjoin_f(*env_rpl, ft_strdup("$"));
	*p_nonchar = str;
}
