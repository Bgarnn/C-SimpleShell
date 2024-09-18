#include "minishell.h"

void	free_path(char *path)
{
	free(path);
	path = NULL;
}
