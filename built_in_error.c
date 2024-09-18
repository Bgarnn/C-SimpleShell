#include "minishell.h"

void	export_err(t_data *data, t_token_node *curr_token)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd("export: `", 2);
	ft_putstr_fd(curr_token->value, 2);
	ft_putendl_fd("': not a valid identifier", 2);
	data->errnum = 1;
	if (data->builtin_parent == 0)
	{
		free_everything(data);
		exit(1);
	}
}

void	unset_err(t_data *data, t_token_node *curr_token)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd("unset: `", 2);
	ft_putstr_fd(curr_token->value, 2);
	ft_putendl_fd("': not a valid identifier", 2);
	data->errnum = 1;
	if (data->builtin_parent == 0)
	{
		free_everything(data);
		exit(1);
	}
}
