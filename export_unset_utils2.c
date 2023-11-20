/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_unset_utils2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plertsir <plertsir@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 15:44:56 by plertsir          #+#    #+#             */
/*   Updated: 2023/11/16 17:01:16 by plertsir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_new_env(t_data *data, t_token_node *curr_token, int *status)
{
	if (before_export(data, curr_token) == FALSE)
	{
		*status = 1;
		return (TRUE);
	}
	else if (search_duplicate(data, curr_token) == TRUE)
		return (TRUE);
	return (FALSE);
}

void	make_new_env(t_data *data, t_token_node *curr_token, char **new_env)
{
	size_t	i;

	i = 0;
	while (i < data->env_row_max - 1)
	{
		new_env[i] = ft_strdup(data->env[i]);
		i++;
	}
	new_env[data->env_row_max - 1] = ft_strdup(curr_token->value);
	free_env(data->env);
	data->env = new_env;
	curr_token = curr_token->next;
}

void	loop_unset(t_data *data, t_token_node *curr_token, char **new_env)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
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
}

int	check_before_unset(t_data *data, t_token_node *curr_token, int *status)
{
	if (before_unset(data, curr_token) == FALSE)
	{
		*status = 1;
		return (TRUE);
	}
	return (FALSE);
}
