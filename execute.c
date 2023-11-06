/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plertsir <plertsir@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 11:27:51 by plertsir          #+#    #+#             */
/*   Updated: 2023/11/02 16:15:33 by plertsir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	len_spl_av(t_token_node *curr_token)
{
	size_t	i;

	i = 0;
	while (curr_token != NULL)
	{
		curr_token = curr_token->next;
		i++;
	}
	return (i);
}

void	go_exec(t_data *data, t_list_node *curr_list)
{
	char	**path_exec;
	size_t	len_av;
	size_t	len_cmd;
	int		i;

	errno = 0;
	len_av = len_spl_av(curr_list->cmd);
	len_cmd = ft_strlen(curr_list->cmd->value);
	path_exec = ft_calloc(len_av + 1, sizeof(char *));
	path_exec[0] = (char *)malloc(len_cmd + 1 * sizeof(char));
	path_cpy(&(*path_exec[0]), curr_list->cmd->value);
	curr_list->cmd = curr_list->cmd->next;
	i = 1;
	while (curr_list->cmd != NULL)
	{
		len_cmd = ft_strlen(curr_list->cmd->value);
		path_exec[i] = (char *)malloc(len_cmd + 1 * sizeof(char));
		path_cpy(path_exec[i], curr_list->cmd->value);
		curr_list->cmd = curr_list->cmd->next;
		i++;
	}
	execve(path_exec[0], path_exec, data->env);
	exit(11);
	// free_2d(spl_av);
	// free_2d(path_exec);
	// free_mem(data, 1);
}
