/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_update_env.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plertsir <plertsir@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 15:06:47 by plertsir          #+#    #+#             */
/*   Updated: 2023/11/15 18:55:09 by plertsir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_oldpwd(t_data *data, char *old_pwd)
{
	char	*tmp;
	int		i;

	i = 0;
	if (old_pwd != NULL)
	{
		while (old_pwd[i] != '=')
			i++;
		tmp = ft_strchr(old_pwd, '=');
		old_pwd = ft_substr(old_pwd, i + 1, ft_strlen(old_pwd) - (size_t)tmp);
		printf("%s\n", old_pwd);
		cd_update(data, old_pwd);
		chdir(old_pwd);
		free(old_pwd);
	}
	else
	{
		ft_putendl_fd("minishell: cd: OLDPWD not set", 2);
		data->errnum = 1;
		if (data->builtin_parent == 0)
			exit(1);
	}
}

void	cd_old(t_data *data)
{
	char	*old_pwd;
	int		i;

	i = 0;
	old_pwd = NULL;
	while (data->env[i])
	{
		if (is_duplicate(data->env[i], "OLDPWD=") == 1)
		{
			old_pwd = data->env[i];
			break ;
		}
		i++;
	}
	add_oldpwd(data, old_pwd);
}

void	cd_err(t_data *data, t_list_node *curr_list, int errno_nb)
{
	if (errno_nb == 13)
		file_error(data, curr_list->cmd->next->value);
	else if (errno_nb == 2)
		cd_path_error(data, curr_list->cmd->next);
	else
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(curr_list->cmd->next->value, 2);
		ft_putendl_fd(": Not a directory", 2);
		data->errnum = 1;
	}
}

void	cd_update_old(t_data *data, char *old_pwd)
{
	int		i;
	char	*new_pwd;

	i = 0;
	while (data->env[i])
	{
		if (is_duplicate(data->env[i], "OLDPWD=") == 1)
		{
			new_pwd = ft_strjoin("OLDPWD=", old_pwd);
			free(data->env[i]);
			data->env[i] = ft_strdup(new_pwd);
			free(new_pwd);
			new_pwd = NULL;
		}
		i++;
	}
}

void	cd_update(t_data *data, char *old_pwd)
{
	int		i;
	char	*new_pwd;
	char	curr_pwd[PATH_MAX];

	i = 0;
	if (!getcwd(curr_pwd, PATH_MAX))
	{
		perror("getcwd");
		free_everything(data);
		exit(1);
	}
	while (data->env[i])
	{
		if (is_duplicate(data->env[i], "PWD=") == 1)
		{
			new_pwd = ft_strjoin("PWD=", curr_pwd);
			free(data->env[i]);
			data->env[i] = ft_strdup(new_pwd);
			free(new_pwd);
			new_pwd = NULL;
		}
		i++;
	}
	cd_update_old(data, old_pwd);
}
