/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plertsir <plertsir@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 18:53:16 by plertsir          #+#    #+#             */
/*   Updated: 2023/11/15 12:43:10 by plertsir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execeve_fail(t_data *data, t_token_node *curr_token)
{
	free_everything(data);
	if (curr_token == NULL)
		exit(0);
	ft_putendl_fd("minishell: execution command error", 2);
	exit(1);
}
