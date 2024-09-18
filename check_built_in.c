#include "minishell.h"

static int	check_builtin_parent2(t_data *data, t_list_node *curr_list)
{
	if (ft_strcmp(curr_list->cmd->value, "export") == 0)
	{
		data->builtin_parent = 1;
		if (export_new_env(data, curr_list->cmd->next) == FALSE)
			return (data->builtin_parent = 1, TRUE);
		data->errnum = 0;
	}
	else if (ft_strcmp(curr_list->cmd->value, "unset") == 0)
	{
		data->builtin_parent = 1;
		if (unset_env(data, curr_list->cmd->next) == FALSE)
			return (data->builtin_parent = 1, TRUE);
		data->errnum = 0;
	}
	else
		return (data->builtin_parent = 0, FALSE);
	return (data->builtin_parent = 1, TRUE);
}

int	check_builtin_parent(t_data *data, t_list_node *curr_list)
{
	if (curr_list->cmd == NULL)
		return (data->builtin_parent = 0, FALSE);
	data->builtin_parent = 1;
	if (check_inout_file(data, curr_list) == FALSE)
		return (data->builtin_parent = 1, TRUE);
	data->builtin_parent = 0;
	if (ft_strcmp(curr_list->cmd->value, "cd") == 0)
	{
		data->builtin_parent = 1;
		change_dir(data, curr_list);
	}
	else if (ft_strcmp(curr_list->cmd->value, "exit") == 0)
	{
		data->builtin_parent = 1;
		go_exit(data, curr_list);
	}
	else if (check_builtin_parent2(data, curr_list) == FALSE)
		return (FALSE);
	return (TRUE);
}

int	check_builtin_child(t_data *data, t_list_node *curr_list)
{
	if (ft_strcmp(curr_list->cmd->value, "pwd") == 0)
		return (get_curr_dir(data), TRUE);
	else if (ft_strcmp(curr_list->cmd->value, "env") == 0)
		return (call_env(data, curr_list->cmd), TRUE);
	else if (ft_strcmp(curr_list->cmd->value, "echo") == 0)
		return (echo_words(data, curr_list->cmd->next), TRUE);
	return (FALSE);
}

static void	before_child_exe2(t_data *data, t_list_node *curr_list)
{
	if (ft_strcmp(curr_list->cmd->value, "export") == 0)
	{
		if (curr_list->cmd->next == NULL)
		{
			declare_env(data);
			free_everything(data);
		}
		else if (is_valid_ident(data, curr_list->cmd->next) == FALSE)
			export_err(data, curr_list->cmd->next);
		else if (curr_list->cmd->next->value[0] == '=')
			export_err(data, curr_list->cmd->next);
		else
			free_everything(data);
		exit(0);
	}
	else if (ft_strcmp(curr_list->cmd->value, "unset") == 0)
	{
		if (curr_list->cmd->next == NULL)
			free_everything(data);
		if (is_valid_unset(data, curr_list->cmd->next) == FALSE)
			unset_err(data, curr_list->cmd->next);
		else
			free_everything(data);
		exit(0);
	}
}

int	before_child_exe(t_data *data, t_list_node *curr_list)
{
	if (ft_strcmp(curr_list->cmd->value, "cd") == 0)
	{
		if (ft_strcmp(curr_list->cmd->next->value, "-") == 0)
			return (cd_old(data), FALSE);
		check_cd_err(data, curr_list);
		return (FALSE);
	}
	else if (ft_strcmp(curr_list->cmd->value, "exit") == 0)
	{
		if (curr_list->cmd->next != NULL)
			go_exit(data, curr_list);
		return (FALSE);
	}
	before_child_exe2(data, curr_list);
	return (TRUE);
}
