#include "minishell.h"

int	get_curr_dir(t_data *data)
{
	char	curr_dir[PATH_MAX];

	if (!getcwd(curr_dir, PATH_MAX))
	{
		perror("getcwd");
		free_everything(data);
		exit(1);
	}
	ft_putendl_fd(curr_dir, STDOUT_FILENO);
	return (data->errnum = 0, TRUE);
}
