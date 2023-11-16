/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_built_in.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plertsir <plertsir@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 16:28:48 by plertsir          #+#    #+#             */
/*   Updated: 2023/11/15 19:05:15 by plertsir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	home_err(t_data *data)
{
	ft_putendl_fd("cd: HOME not set", 2);
	data->errnum = 1;
}

static void	check_home(t_data *data, t_token_node *curr_token)
{
	int		i;
	char	*home;
	char	*tmp;
	char	curr_pwd[PATH_MAX];

	i = 0;
	home = find_home(data);
	if (!home)
		home_err(data);
	else
	{
		i = 0;
		while (home[i] != '=')
			i++;
		tmp = ft_strchr(home, '=');
		home = ft_substr(home, i + 1, ft_strlen(home) - (size_t)tmp);
		getcwd(curr_pwd, PATH_MAX);
		chdir(home);
		free(curr_token->value);
		curr_token->value = ft_strjoin("OLDPWD=", curr_pwd);
		export_new_env(data, curr_token);
		cd_update(data, curr_pwd);
		free(home);
		data->errnum = 0;
	}
}

void	change_dir(t_data *data, t_list_node *curr_list)
{
	int		i;
	char	*tmp;
	char	curr_pwd[PATH_MAX];

	i = 0;
	if (curr_list->cmd->next != NULL)
	{
		getcwd(curr_pwd, PATH_MAX);
		if (ft_strcmp(curr_list->cmd->next->value, "-") == 0)
			cd_old(data);
		else if (chdir(curr_list->cmd->next->value) != -1)
		{
			free(curr_list->cmd->next->value);
			tmp = ft_strjoin("OLDPWD=", curr_pwd);
			curr_list->cmd->next->value = tmp;
			export_new_env(data, curr_list->cmd->next);
			cd_update(data, curr_pwd);
			data->errnum = 0;
		}
		else
			cd_err(data, curr_list, errno);
	}
	else
		check_home(data, curr_list->cmd);
}

void	directory_err(t_data *data, t_list_node *curr_list)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd("cd: ", 2);
	ft_putstr_fd(curr_list->cmd->next->value, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd("Not a directory", 2);
	data->errnum = 1;
	free_everything(data);
	exit(1);
}

void	check_cd_err(t_data *data, t_list_node *curr_list)
{
	if (curr_list->cmd->next != NULL)
	{
		if (chdir(curr_list->cmd->next->value) != -1)
			data->errnum = 0;
		else if (errno == 13)
			file_error(data, curr_list->cmd->next->value);
		else if (errno == 2)
			cd_path_error(data, curr_list->cmd->next);
		else
			directory_err(data, curr_list);
	}
	else
	{
		check_home(data, curr_list->cmd);
		free_everything(data);
		if (data->errnum == 1)
			exit(1);
		exit(0);
	}
}
