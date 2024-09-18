#include "minishell.h"

int	check_inout_file(t_data *data, t_list_node *curr_list)
{
	if (curr_list->infile != NULL)
	{
		if (check_infile(curr_list->infile, data) == FALSE)
			return (FALSE);
	}
	if (curr_list->outfile != NULL)
	{
		if (check_outfile(curr_list->outfile, data) == FALSE)
			return (FALSE);
	}
	return (TRUE);
}

static void	pipe_next_child(int *pipe_w, int *pipe_r)
{
	if (dup2(*pipe_r, STDIN_FILENO) == -1)
		ft_putendl_fd("duplicate read-end error", 2);
	close(*pipe_r);
	close(*pipe_w);
}

static void	check_everything(t_list_node *curr_list, t_data *data, \
int *pipe_w, int *pipe_r)
{
	check_inout_file(data, curr_list);
	if (curr_list->outfile == NULL && curr_list->next != NULL)
	{
		if (dup2(*pipe_w, STDOUT_FILENO) == -1)
			ft_putendl_fd("duplicate write-end error", 2);
		close(*pipe_r);
		close(*pipe_w);
	}
	if (curr_list->cmd != NULL && check_builtin_child(data, curr_list) == FALSE)
	{
		if (data->builtin_parent != 1 && \
		before_child_exe(data, curr_list) == TRUE)
			get_path(curr_list, data);
	}
	exit(0);
}

void	fork_child(t_data *data, t_list_node *curr_list, int *pipe_w, \
int *pipe_r)
{
	if (curr_list->next != NULL)
		data->child_born = 1;
	if (data->child_born == 0 && check_builtin_parent(data, curr_list) \
	== TRUE)
	{
		return ;
	}
	data->pid[data->index] = fork();
	if (data->pid[data->index] == 0)
		check_everything(curr_list, data, pipe_w, pipe_r);
	else if (curr_list->next != NULL)
		pipe_next_child(pipe_w, pipe_r);
}
