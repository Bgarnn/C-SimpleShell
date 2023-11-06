/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plertsir <plertsir@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 10:40:10 by plertsir          #+#    #+#             */
/*   Updated: 2023/11/02 17:06:19 by plertsir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_path(char *path)
{
	free(path);
	path = NULL;
}

static char	**split_path(t_data *data, char *path)
{
	char	**path_split;

	(void)data;
	path_split = ft_split(path, ':');
	if (!path_split)
	{
		exit(1);
		// free_path(path);
		// free_2d(av);
		// free_mem(data, 1);
	}
	return (path_split);
}

static void	check_slash(t_data *data, t_list_node *curr_list)
{
	if (ft_strchr(curr_list->cmd->value, '/') != NULL)
	{
		if (access(curr_list->cmd->value, X_OK) != -1)
			go_exec(data, curr_list);
		else if (errno == 13)
		{
			ft_putstr_fd(curr_list->cmd->value, 2);
			ft_putstr_fd(": ", 2);
			ft_putendl_fd(strerror(errno), 2);
			// free_2d(spl_av);
			exit(126);
			// free_mem(data, 126);
		}
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

	errno = 0;
	check_slash(data, curr_list);
	path = ft_substr(path_exec, 5, data->len_path);
	path2 = split_path(data, path);
	free_path(path);
	i = 0;
	while (path2[i])
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
		i++;
	}
	// free_2d(path2);
	if (errno != 0)
		cmd_error(data, curr_list->cmd);
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
		data->len_path = ft_strlen(data->env[i]);
		status = find_path(data->env[i]);
		if (status == 1)
		{
			path_exec = data->env[i];
			break ;
		}
		i++;
	}
	if (status == 0)
		path_error(data, curr_list->cmd);
	ext_path(curr_list, data, path_exec);
}
