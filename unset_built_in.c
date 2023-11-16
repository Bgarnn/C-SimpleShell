/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_built_in.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plertsir <plertsir@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 09:38:38 by plertsir          #+#    #+#             */
/*   Updated: 2023/11/15 14:40:20 by plertsir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid_unset(t_data *data, t_token_node *curr_token)
{
	int	i;

	(void)data;
	i = 0;
	if (ft_isalpha(curr_token->value[0]) == 0 && (curr_token->value[i] != '_'))
		return (FALSE);
	while (curr_token->value[i])
	{
		if ((ft_isalnum(curr_token->value[i]) == 0 && \
		(curr_token->value[i] != '_')) || curr_token->value[i] == '=')
			return (FALSE);
		i++;
	}
	return (TRUE);
}

static int	is_need_to_unset(t_data *data, t_token_node *curr_token)
{
	size_t	i;

	i = 0;
	while (data->env[i])
	{
		if (is_duplicate(data->env[i], curr_token->value) == 1)
			return (TRUE);
		i++;
	}
	return (FALSE);
}

static int	before_unset(t_data *data, t_token_node *curr_token)
{
	if (curr_token == NULL)
		return (FALSE);
	else if (is_valid_unset(data, curr_token) == FALSE)
		return (FALSE);
	else if (is_need_to_unset(data, curr_token) == FALSE)
		return (FALSE);
	return (TRUE);
}

int	unset_env(t_data *data, t_token_node *curr_token)
{
	char	**new_env;
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (before_unset(data, curr_token) == FALSE)
		return (FALSE);
	data->env_row_max = data->env_row_max - 1;
	new_env = (char **)ft_calloc(sizeof(char *), data->env_row_max + 1);
	if (!new_env)
		return (ft_putendl_fd("malloc error", 2), FALSE);
	while (i < data->env_row_max)
	{
		if (is_duplicate(data->env[j], curr_token->value) == 1)
			j++;
		new_env[i] = ft_strdup(data->env[j]);
		i++;
		j++;
	}
	free_env(data->env);
	data->env = new_env;
	return (TRUE);
}
