/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_built_in.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plertsir <plertsir@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 11:23:15 by plertsir          #+#    #+#             */
/*   Updated: 2023/11/17 17:15:42 by plertsir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	declare_env(t_data *data)
{
	size_t	i;
	char	**env_copy;

	i = 0;
	env_copy = (char **)ft_calloc(sizeof(char *), data->env_row_max + 1);
	if (!env_copy)
		return (ft_putendl_fd("malloc error", 2), FALSE);
	while (i < data->env_row_max)
	{
		env_copy[i] = join_quote(ft_strdup(data->env[i]));
		i++;
	}
	sort_env(data, env_copy);
	i = 0;
	while (env_copy[i])
	{
		printf("declare -x %s\n", env_copy[i]);
		i++;
	}
	free_env(env_copy);
	return (TRUE);
}

int	is_duplicate(char *s1, char *s2)
{
	int		i;
	int		s1_len;

	i = 0;
	s1_len = len_env(s1);
	while ((s1_len != 0 || s2[i]) && s2[i] != '=')
	{
		if (s1[i] != s2[i])
			return (0);
		i++;
		s1_len--;
	}
	return (1);
}

int	search_duplicate(t_data *data, t_token_node *curr_token)
{
	size_t	i;

	i = 0;
	while (data->env[i])
	{
		if (is_duplicate(data->env[i], curr_token->value) == 1)
		{
			free(data->env[i]);
			data->env[i] = ft_strdup(curr_token->value);
			return (TRUE);
		}
		i++;
	}
	return (FALSE);
}

int	before_export(t_data *data, t_token_node *curr_token)
{
	if (curr_token == NULL || \
	ft_strchr(curr_token->value, '=') == NULL)
	{
		if (is_valid_ident(data, curr_token) == FALSE)
			return (export_err(data, curr_token), FALSE);
		return (FALSE);
	}
	else if (is_valid_ident(data, curr_token) == FALSE)
		return (export_err(data, curr_token), FALSE);
	else if (curr_token->value[0] == '=')
		return (FALSE);
	return (TRUE);
}

int	export_new_env(t_data *data, t_token_node *curr_token)
{
	char	**new_env;
	int		status;

	status = 0;
	if (curr_token == NULL)
		declare_env(data);
	while (curr_token != NULL)
	{
		if (check_new_env(data, curr_token, &status) == TRUE)
		{
			curr_token = curr_token->next;
			continue ;
		}
		data->env_row_max += 1;
		new_env = (char **)ft_calloc(sizeof(char *), data->env_row_max + 1);
		if (!new_env)
			return (ft_putendl_fd("malloc error", 2), FALSE);
		make_new_env(data, curr_token, new_env);
		curr_token = curr_token->next;
	}
	if (status == 1)
		return (FALSE);
	return (TRUE);
}
