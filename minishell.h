# include <stdio.h>
# include <limits.h>
# include <signal.h>
# include <fcntl.h>
# include <sys/errno.h>
# include <sys/wait.h>
# include <string.h>
# include "libft/libft.h"
# include <readline/readline.h>
# include <readline/history.h>


#ifndef MINISHELL_H
# define MINISHELL_H

typedef struct sigaction	t_sigaction;

typedef enum e_token_mark
{
	m_undefined,
	m_heredoc,
	m_out_append,
	m_out_trunc,
	m_infile,
	m_cmd,
	m_arg,
	m_pipe
}			t_token_mark;

typedef struct s_token_node
{
	t_token_mark		mark;
	char				*type;
	char				*value;
	struct s_token_node	*next;
}						t_token_node;

typedef struct s_token_ptr
{
	t_token_node		*head;
	t_token_node		*tail;
}						t_token_ptr;

typedef struct s_list_ptr
{
	t_token_ptr		infile;
	t_token_ptr		outfile;
	t_token_ptr		cmd;
	struct s_list_ptr	*next;
}						t_list_ptr;

typedef struct s_list_node
{
	t_token_node		*infile;
	t_token_node		*outfile;
	t_token_node		*cmd;
	struct s_list_node	*next;
}						t_list_node;

typedef struct s_data
{
	int					errnum;
	char				**env;
	size_t				env_row_max;
	t_token_ptr			unorganized_token;
	t_token_ptr			organized_token;
	// t_list_node			**grouped_token;
}						t_data;

int	g_signal;


#endif