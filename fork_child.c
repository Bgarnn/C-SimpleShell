/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plertsir <plertsir@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 15:14:15 by plertsir          #+#    #+#             */
/*   Updated: 2023/11/02 18:09:02 by plertsir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pipe_next_child(int *pipe_w, int *pipe_r)
{
	if (dup2(*pipe_r, STDIN_FILENO) == -1)
		dprintf(2, "duplicate infile error\n");
	close(*pipe_r);
	close(*pipe_w);
}

void	check_everything(t_list_node *curr_list, t_data *data, \
int *pipe_w, int *pipe_r)
{
	if (curr_list->infile != NULL)
		check_infile(curr_list->infile, data);
	if (curr_list->outfile != NULL)
		check_outfile(curr_list->outfile, data);
	else if (curr_list->next != NULL)
	{
		if (dup2(*pipe_w, STDOUT_FILENO) == -1)
			dprintf(2, "duplicate outfile error\n");
		close(*pipe_r);
		close(*pipe_w);
	}
	if (curr_list->cmd != NULL)
		get_path(curr_list, data);
	else
		exit(0);
}

void	fork_child(t_data *data, t_list_node *curr_list, int *pipe_w, \
int *pipe_r)
{
	if (curr_list->next == NULL)
	{
		if (check_builtin(data, curr_list) == TRUE)
		{
			data->builtin_parent = 1;
			return ;
		}
	}
	data->pid[data->index] = fork();
	if (data->pid[data->index] == 0)
	{
		check_everything(curr_list, data, pipe_w, pipe_r);
	}
	else if (curr_list->next != NULL)
		pipe_next_child(pipe_w, pipe_r);
}
