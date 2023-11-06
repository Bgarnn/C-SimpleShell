/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plertsir <plertsir@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 16:28:48 by plertsir          #+#    #+#             */
/*   Updated: 2023/11/02 18:11:34 by plertsir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//Parent builtin: cd, export, unset, exit
//Child builtin: echo, pwd, env

int	change_dir(t_data *data, t_list_node *curr_list)
{
	if (chdir(curr_list->cmd->next->value) != -1)
		data->errnum = 0;
	else if (errno == 13)
	{
		ft_putstr_fd(curr_list->cmd->next->value, 2);
		ft_putstr_fd(": ", 2);
		ft_putendl_fd(strerror(errno), 2);
		data->errnum = 1;
	}
	else if (errno == 2)
	{
		ft_putstr_fd(curr_list->cmd->next->value, 2);
		ft_putstr_fd(": ", 2);
		ft_putendl_fd("No such file or directory", 2);
		data->errnum = 1;
	}
	else
	{
		ft_putstr_fd("cd", 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(curr_list->cmd->next->value, 2);
		ft_putstr_fd(": ", 2);
		ft_putendl_fd("Not a directory", 2);
		data->errnum = 1;
	}
	return (0);
}

int	check_builtin(t_data *data, t_list_node *curr_list)
{
	if (curr_list->cmd == NULL)
		return (FALSE);
	if (ft_strcmp(curr_list->cmd->value, "cd") == 0)
	{
		change_dir(data, curr_list);
		return (TRUE);
	}
	else
		return (FALSE);
	return (0);
}
