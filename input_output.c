/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_output.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plertsir <plertsir@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/29 14:22:20 by plertsir          #+#    #+#             */
/*   Updated: 2023/11/02 14:59:20 by plertsir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_infile(t_token_node *curr_token, t_data *data)
{
	while (curr_token != NULL)
	{
		if (curr_token->mark == m_infile)
		{
			data->fd_in = open(curr_token->value, O_RDONLY);
			if (data->fd_in == -1)
				file_error(data, curr_token->value);
		}
		if (curr_token->mark == m_heredoc)
			data->fd_in = curr_token->here_doc_fd;
		curr_token = curr_token->next;
	}
	dup2(data->fd_in, STDIN_FILENO);
	close(data->fd_in);
	return (0);
}

int	check_outfile(t_token_node *curr_token, t_data *data)
{
	while (curr_token != NULL)
	{
		if (curr_token->mark == m_out_trunc)
			data->fd_out = open(curr_token->value, O_TRUNC | O_WRONLY | \
			O_CREAT, 0644);
		if (curr_token->mark == m_out_append)
			data->fd_out = open(curr_token->value, O_APPEND | O_WRONLY | \
			O_CREAT, 0644);
		if (data->fd_out == -1)
			file_error(data, curr_token->value);
		curr_token = curr_token->next;
	}
	dup2(data->fd_out, STDOUT_FILENO);
	close(data->fd_out);
	return (0);
}

int	open_heredoc(t_token_node *curr_token)
{
	char	*in_doc;
	int		pipe_fd[2];

	in_doc = NULL;
	if (pipe(pipe_fd) == -1)
		return (ft_putstr_fd("Pipe here_doc fail\n", 2), -1);
	while (1)
	{
		in_doc = readline("> ");
		if (!in_doc || ft_strcmp(in_doc, curr_token->value) == 0)
		{
			free(in_doc);
			in_doc = NULL;
			break ;
		}
		ft_putendl_fd(in_doc, pipe_fd[1]);
		free(in_doc);
		in_doc = NULL;
	}
	close(pipe_fd[1]);
	return (pipe_fd[0]);
}

int	check_here_doc(t_list_node *curr_list)
{
	t_token_node	*curr_token;

	while (curr_list != NULL)
	{
		curr_token = curr_list->infile;
		while (curr_token != NULL)
		{
			if (curr_token->mark == m_heredoc)
			{
				curr_token->here_doc_fd = open_heredoc(curr_token);
				if (curr_token->here_doc_fd == -1)
					return (FALSE);
			}
			curr_token = curr_token->next;
		}
		curr_list = curr_list->next;
	}
	return (TRUE);
}
