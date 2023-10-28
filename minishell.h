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

typedef struct s_data
{
	int			err_num;
}				t_data;

int	g_signal;


#endif