/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_error.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plertsir <plertsir@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 23:06:25 by plertsir          #+#    #+#             */
/*   Updated: 2023/11/15 10:13:26 by plertsir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	export_err(t_data *data, t_token_node *curr_token)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd("export: `", 2);
	ft_putstr_fd(curr_token->value, 2);
	ft_putendl_fd("': not a valid identifier", 2);
	data->errnum = 1;
	free_everything(data);
	exit(1);
}

void	unset_err(t_data *data, t_token_node *curr_token)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd("unset: `", 2);
	ft_putstr_fd(curr_token->value, 2);
	ft_putendl_fd("': not a valid identifier", 2);
	data->errnum = 1;
	free_everything(data);
	exit(1);
}
