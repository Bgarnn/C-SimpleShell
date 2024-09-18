#include "minishell.h"

static int	exit_err(t_data *data, t_list_node *curr_list, int *status_err, \
unsigned char status)
{
	if (*status_err != 0)
	{
		if (*status_err == 1)
		{
			ft_putendl_fd("exit", STDOUT_FILENO);
			ft_putstr_fd("minishell: exit: ", STDOUT_FILENO);
			ft_putstr_fd(curr_list->cmd->next->value, STDOUT_FILENO);
			ft_putstr_fd(": ", STDOUT_FILENO);
			ft_putendl_fd("numeric argument required", STDOUT_FILENO);
			free_everything(data);
			exit(255);
		}
		else if (*status_err == 2)
		{
			ft_putendl_fd("exit", STDOUT_FILENO);
			ft_putstr_fd("minishell: exit: ", STDOUT_FILENO);
			ft_putendl_fd("too many arguments", STDOUT_FILENO);
			data->errnum = (int)status;
			return (FALSE);
		}
	}
	return (TRUE);
}

void	go_exit(t_data *data, t_list_node *curr_list)
{
	unsigned char	status;
	int				status_err;

	if (curr_list->cmd->next != NULL)
	{
		status_err = check_status(curr_list->cmd->next, &status);
		if (status_err != 0)
		{
			if (exit_err(data, curr_list, &status_err, status) == FALSE)
				return ;
		}
		else
		{
			data->errnum = (int)status;
			if (data->builtin_parent == 1)
				ft_putendl_fd("exit", STDOUT_FILENO);
			free_everything(data);
			exit(data->errnum);
		}
	}
	if (data->builtin_parent == 1)
		ft_putendl_fd("exit", STDOUT_FILENO);
	free_everything(data);
	exit(0);
}
