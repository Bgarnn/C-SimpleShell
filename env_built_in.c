/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_built_in.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plertsir <plertsir@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 15:11:54 by plertsir          #+#    #+#             */
/*   Updated: 2023/11/15 13:39:27 by plertsir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	call_env(t_data *data, t_token_node *curr_token)
{
	size_t	i;

	i = 0;
	if (curr_token->next != NULL)
	{
		ft_putstr_fd("minishell: env: ", 2);
		ft_putendl_fd("too many arguments", 2);
		if (data->builtin_parent == 1)
			data->errnum = 1;
		else
		{
			free_everything(data);
			exit(1);
		}
	}
	while (data->env[i])
	{
		ft_putendl_fd(data->env[i], 1);
		i++;
	}
}
