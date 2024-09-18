#include "minishell.h"

static void	check_numchild(t_data *data)
{
	t_list_node	*run;
	int			i;

	i = 0;
	run = data->grouped_token;
	while (run != 0)
	{
		run = run->next;
		i++;
	}
	data->num_child = i;
}

static int	prep_before_fork(t_data *data, t_list_node *curr_list)
{
	check_numchild(data);
	data->pid = malloc(sizeof(int) * data->num_child);
	if (!data->pid)
		return (FALSE);
	data->stdin_copy = dup(STDIN_FILENO);
	data->stdout_copy = dup(STDOUT_FILENO);
	if (check_here_doc(curr_list) == FALSE)
		return (FALSE);
	return (TRUE);
}

static void	wait_child(t_data *data)
{
	int		i;

	i = 0;
	while (i < data->num_child)
		waitpid(data->pid[i++], &data->errnum, WUNTRACED);
	free(data->pid);
	data->pid = NULL;
	if (data->builtin_parent != 1)
		data->errnum = WEXITSTATUS(data->errnum);
}

static void	restore_fd(t_data *data)
{
	dup2(data->stdin_copy, STDIN_FILENO);
	dup2(data->stdout_copy, STDOUT_FILENO);
	close(data->stdin_copy);
	close(data->stdout_copy);
}

int	first_execute(t_data *data)
{
	int			fd_pipe[2];
	t_list_node	*curr_list;

	data->builtin_parent = 0;
	data->child_born = 0;
	curr_list = data->grouped_token;
	data->index = 0;
	if (prep_before_fork(data, curr_list) == FALSE)
		return (FALSE);
	while (curr_list != NULL)
	{
		if (pipe(fd_pipe) == -1)
			return (ft_putstr_fd("pipe error\n", 2), FALSE);
		fork_child(data, curr_list, &fd_pipe[1], &fd_pipe[0]);
		close(fd_pipe[0]);
		close(fd_pipe[1]);
		curr_list = curr_list->next;
		data->index++;
	}
	restore_fd(data);
	wait_child(data);
	data->pid = 0;
	return (TRUE);
}
