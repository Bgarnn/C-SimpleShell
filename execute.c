/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plertsir <plertsir@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 11:27:51 by plertsir          #+#    #+#             */
/*   Updated: 2023/11/15 12:40:52 by plertsir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_path_exec(char **ptr)
{
	size_t	i;

	i = 0;
	while (ptr[i])
	{
		free(ptr[i]);
		ptr[i] = NULL;
		i++;
	}
	free(ptr);
	ptr = NULL;
}

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

static char	**malloc_path_exec(t_data *data, size_t len_av, size_t len_cmd)
{
	char	**path_exec;

	path_exec = ft_calloc(len_av + 1, sizeof(char *));
	if (!path_exec)
	{
		ft_putendl_fd("malloc error", 2);
		free_everything(data);
		exit(1);
	}
	path_exec[0] = (char *)malloc(len_cmd + 1 * sizeof(char));
	if (!path_exec[0])
	{
		ft_putendl_fd("malloc error", 2);
		free_everything(data);
		exit(1);
	}
	return (path_exec);
}

static void	path_err(t_data *data, char **path_exec)
{
	free_path_exec(path_exec);
	ft_putendl_fd("malloc error", 2);
	free_everything(data);
	exit(1);
}

void	go_exec(t_data *data, t_list_node *curr_list)
{
	char	**path_exec;
	size_t	len_av;
	size_t	len_cmd;

	len_av = len_spl_av(curr_list->cmd);
	len_cmd = ft_strlen(curr_list->cmd->value);
	path_exec = malloc_path_exec(data, len_av, len_cmd);
	path_cpy(&(*path_exec[0]), curr_list->cmd->value);
	curr_list->cmd = curr_list->cmd->next;
	data->index = 1;
	while (curr_list->cmd != NULL)
	{
		len_cmd = ft_strlen(curr_list->cmd->value);
		path_exec[data->index] = (char *)malloc(len_cmd + 1 * sizeof(char));
		if (!path_exec[data->index])
			path_err(data, path_exec);
		path_cpy(path_exec[data->index], curr_list->cmd->value);
		curr_list->cmd = curr_list->cmd->next;
		data->index++;
	}
	execve(path_exec[0], path_exec, data->env);
	execeve_fail(data, curr_list->cmd);
}
