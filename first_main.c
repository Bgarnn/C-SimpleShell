/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   first_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plertsir <plertsir@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 15:18:25 by plertsir          #+#    #+#             */
/*   Updated: 2023/11/02 18:08:58 by plertsir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_numchild(t_data *data)
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

int	prep_before_fork(t_data *data, t_list_node *curr_list)
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

void	wait_child(t_data *data)
{
	int		i;

	i = 0;
	while (i < data->num_child)
		waitpid(data->pid[i++], &data->errnum, WUNTRACED);
	free(data->pid);
	data->pid = NULL;
	dup2(data->stdin_copy, STDIN_FILENO);
	dup2(data->stdout_copy, STDOUT_FILENO);
	if (data->builtin_parent != 1)
		data->errnum = WEXITSTATUS(data->errnum);
}

int	first_execute(t_data *data)
{
	int			fd_pipe[2];
	t_list_node	*curr_list;

	data->builtin_parent = 0;
	curr_list = data->grouped_token;
	data->index = 0;
	if (prep_before_fork(data, curr_list) == FALSE)
		return (FALSE);
	while (curr_list != NULL)
	{
		if (pipe(fd_pipe) == -1)
			return (ft_putstr_fd("pipe error\n", 2), FALSE);
		fork_child(data, curr_list, &fd_pipe[1], &fd_pipe[0]);
		curr_list = curr_list->next;
		data->index++;
	}
	wait_child(data);
	// free_everything(data);
	return (TRUE);
}
