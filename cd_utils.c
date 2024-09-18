#include "minishell.h"

char	*find_home(t_data *data)
{
	int	i;

	i = 0;
	while (data->env[i])
	{
		if (is_duplicate(data->env[i], "HOME") == 1)
			return (data->env[i]);
		i++;
	}
	return (NULL);
}

void	old_pwd_check(t_data *data, t_list_node *curr_list, char *curr_pwd)
{
	char	*tmp;

	cd_old(data);
	free(curr_list->cmd->next->value);
	tmp = ft_strjoin("OLDPWD=", curr_pwd);
	curr_list->cmd->next->value = tmp;
	export_new_env(data, curr_list->cmd->next);
}
