/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plertsir <plertsir@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 10:40:10 by plertsir          #+#    #+#             */
/*   Updated: 2023/11/16 21:45:06 by plertsir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ext_path_err(t_data *data, t_list_node *curr_list, int err_num)
{
	if (err_num == 13)
		permis_error(data, curr_list->cmd);
	cmd_error(data, curr_list->cmd);
}

char	**split_path(t_data *data, char *path, t_list_node *curr_list)
{
	char	**path_split;

	if (path == NULL)
		path_error(data, curr_list->cmd);
	path_split = ft_split(path, ':');
	if (!path_split)
	{
		free_everything(data);
		free_path(path);
		exit(1);
	}
	free_path(path);
	return (path_split);
}

void	check_slash(t_data *data, t_list_node *curr_list)
{
	if (ft_strchr(curr_list->cmd->value, '/') != NULL)
	{
		if (is_directory(curr_list->cmd->value) == 1)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(curr_list->cmd->value, 2);
			ft_putstr_fd(": ", 2);
			ft_putendl_fd(" is a directory", 2);
			free_everything(data);
			exit(126);
		}
		if (access(curr_list->cmd->value, X_OK) != -1)
			go_exec(data, curr_list);
		else if (errno == 13)
			permis_error(data, curr_list->cmd);
		else
			path_error(data, curr_list->cmd);
	}
}

static void	ext_path(t_list_node *curr_list, t_data *data, char *path_exec)
{
	char	*path;
	char	*tmp;
	char	**path2;
	int		i;

	check_slash(data, curr_list);
	path2 = split_path(data, ft_substr(path_exec, 5, data->len_path), \
	curr_list);
	i = -1;
	if (is_bash(curr_list, data) == FALSE)
		bash_err(data, curr_list->cmd);
	while (path2[++i])
	{
		tmp = ft_strjoin(path2[i], "/");
		path = ft_strjoin(tmp, curr_list->cmd->value);
		free(tmp);
		if (access(path, X_OK) != -1)
		{
			free(curr_list->cmd->value);
			curr_list->cmd->value = path;
			go_exec(data, curr_list);
		}
		free_path(path);
	}
	ext_path_err(data, curr_list, errno);
}

void	get_path(t_list_node *curr_list, t_data *data)
{
	int		i;
	int		status;
	char	*path_exec;

	i = 0;
	status = 0;
	path_exec = NULL;
	while (data->env[i])
	{
		status = find_path(data->env[i]);
		data->len_path = ft_strlen(data->env[i]);
		if (status == 1 && data->len_path == 5)
			break ;
		if (status == 1)
		{
			path_exec = data->env[i];
			break ;
		}
		i++;
	}
	if (access(curr_list->cmd->value, X_OK) == -1 && status == 0)
		path_error(data, curr_list->cmd);
	ext_path(curr_list, data, path_exec);
}
