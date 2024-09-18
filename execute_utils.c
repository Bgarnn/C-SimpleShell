#include "minishell.h"

void	execeve_fail(t_data *data, t_token_node *curr_token)
{
	free_everything(data);
	if (curr_token == NULL)
		exit(0);
	ft_putendl_fd("minishell: execution command error", 2);
	exit(1);
}
