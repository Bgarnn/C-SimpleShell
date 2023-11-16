/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_memory.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plertsir <plertsir@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 09:48:33 by plertsir          #+#    #+#             */
/*   Updated: 2023/11/07 21:22:54 by plertsir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_everything(t_data *data)
{
	if (data->grouped_token != NULL)
		free_grouped_token(data->grouped_token);
	if (data->env != NULL)
		free_env(data->env);
	if (data->pid != NULL)
	{
		free(data->pid);
		data->pid = NULL;
	}
	return ;
}
