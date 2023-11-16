/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_built_in.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plertsir <plertsir@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 11:23:15 by plertsir          #+#    #+#             */
/*   Updated: 2023/11/15 18:46:06 by plertsir         ###   ########.fr       */
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
	size_t	s1_len;

	i = 0;
	s1_len = len_env(s1);
	while (s1_len > 0)
	{
		if (s1[i] != s2[i])
			return (0);
		i++;
		s1_len--;
	}
	return (1);
}

static int	search_duplicate(t_data *data, t_token_node *curr_token)
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

static int	before_export(t_data *data, t_token_node *curr_token)
{
	if (curr_token == NULL || \
	ft_strchr(curr_token->value, '=') == NULL)
		return (FALSE);
	else if (is_valid_ident(data, curr_token) == FALSE)
		return (FALSE);
	else if (curr_token->value[0] == '=')
		return (FALSE);
	return (TRUE);
}

int	export_new_env(t_data *data, t_token_node *curr_token)
{
	char	**new_env;
	size_t	i;

	if (before_export(data, curr_token) == FALSE)
		return (FALSE);
	i = 0;
	if (search_duplicate(data, curr_token) == TRUE)
		return (TRUE);
	data->env_row_max += 1;
	new_env = (char **)ft_calloc(sizeof(char *), data->env_row_max + 1);
	if (!new_env)
		return (ft_putendl_fd("malloc error", 2), FALSE);
	while (i < data->env_row_max - 1)
	{
		new_env[i] = ft_strdup(data->env[i]);
		i++;
	}
	new_env[data->env_row_max - 1] = ft_strdup(curr_token->value);
	free_env(data->env);
	data->env = new_env;
	return (TRUE);
}
