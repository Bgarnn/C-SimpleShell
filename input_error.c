/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plertsir <plertsir@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/29 11:48:09 by plertsir          #+#    #+#             */
/*   Updated: 2023/11/16 10:14:28 by plertsir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	file_error(t_data *data, char *file)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(file, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(strerror(errno), 2);
	if (data->builtin_parent == 1)
		data->errnum = 1;
	else
	{
		free_everything(data);
		exit(1);
	}
}

void	path_error(t_data *data, t_token_node *curr_token)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(curr_token->value, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd("No such file or directory", 2);
	if (data->builtin_parent == 1)
		data->errnum = 127;
	else
	{
		free_everything(data);
		exit(127);
	}
}

void	cmd_error(t_data *data, t_token_node *curr_token)
{
	(void)data;
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(curr_token->value, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd("command not found", 2);
	free_everything(data);
	exit(127);
}

void	cd_path_error(t_data *data, t_token_node *curr_token)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd("cd: ", 2);
	ft_putstr_fd(curr_token->value, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd("No such file or directory", 2);
	data->errnum = 1;
	if (data->builtin_parent == 0)
	{
		free_everything(data);
		exit(1);
	}
}

void	permis_error(t_data *data, t_token_node *curr_token)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(curr_token->value, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(strerror(errno), 2);
	free_everything(data);
	exit(126);
}
