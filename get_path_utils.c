#include "minishell.h"

int	is_bash(t_list_node *curr_list, t_data *data)
{
	if ((curr_list->cmd->value[0] == '.' && curr_list->cmd->next == NULL)\
	|| (curr_list->cmd->value[0] == '.' && curr_list->cmd->value[1] == '.'))
		return (FALSE);
	else if (curr_list->cmd->value[0] == '.' && \
	curr_list->cmd->value[1] == '\0')
	{
		go_exec(data, curr_list);
	}
	return (TRUE);
}

void	bash_err(t_data *data, t_token_node *curr_token)
{
	if (curr_token->value[1] != '\0')
		cmd_error(data, curr_token);
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(curr_token->value, 2);
	ft_putendl_fd(": filename argument required", 2);
	ft_putendl_fd(".: usage: . filename [arguments]", 2);
	free_everything(data);
	exit(2);
}
