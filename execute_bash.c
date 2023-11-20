/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_bash.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plertsir <plertsir@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 10:20:40 by plertsir          #+#    #+#             */
/*   Updated: 2023/11/16 15:43:27 by plertsir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_file_exist(const char *path)
{
	int			isfile_exist;
	struct stat	buff;

	isfile_exist = 0;
	isfile_exist = stat(path, &buff);
	if (isfile_exist == 0)
		return (TRUE);
	return (FALSE);
}

void	bash_permis_err(t_data *data, char *path)
{
	char	curr_pwd[PATH_MAX];
	char	*tmp;
	char	*new_tmp;

	getcwd(curr_pwd, PATH_MAX);
	new_tmp = ft_strjoin(curr_pwd, "/");
	tmp = new_tmp;
	new_tmp = ft_strjoin(new_tmp, path);
	free(tmp);
	if (access(new_tmp, R_OK) == -1)
	{
		free(new_tmp);
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": ", 2);
		ft_putendl_fd(strerror(errno), 2);
		free_everything(data);
		exit(1);
	}
}

void	bash_check_err(t_data *data, char *path)
{
	if (is_directory(path) == 1)
	{
		ft_putstr_fd("minishell: .: ", 2);
		ft_putstr_fd(path, 2);
		ft_putendl_fd(": is a directory", 2);
		free_everything(data);
		exit(1);
	}
	else if (is_file_exist(path) == FALSE)
	{
		ft_putstr_fd("minishell: .: ", 2);
		ft_putstr_fd(path, 2);
		ft_putendl_fd(": No such file or directory", 2);
		free_everything(data);
		exit(1);
	}
}

void	execve_bash(t_data *data, t_token_node *curr_token)
{
	char	*path_exec_new[3];

	path_exec_new[0] = "/bin/bash";
	path_exec_new[1] = curr_token->value;
	if (curr_token->value[0] == '.')
	{
		path_exec_new[1] = curr_token->next->value;
		bash_check_err(data, path_exec_new[1]);
		bash_permis_err(data, path_exec_new[1]);
	}
	path_exec_new[2] = NULL;
	execve(path_exec_new[0], path_exec_new, data->env);
	execeve_fail(data, curr_token);
}

void	is_bash_exe(t_data *data, t_token_node	*head_cmd)
{
	if ((ft_strrchr(head_cmd->value, '.') != NULL) || (errno == ENOEXEC))
		execve_bash(data, head_cmd);
	execeve_fail(data, head_cmd);
}
