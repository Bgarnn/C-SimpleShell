#include "minishell.h"

void	free_everything(t_data *data)
{
	if (data->grouped_token != NULL)
		free_grouped_token(data->grouped_token);
	if (data->env != NULL)
		free_env(data->env);
	if (data->pid != NULL)
	{
		free(data->pid);
		data->pid = NULL;
	}
	return ;
}
