#include "minishell.h"

static void	env_init(t_data *data, char **envp)
{
	int	i;
	int	j;
	int	row;

	i = 0;
	j = 0;
	row = row_count(envp);
	data->env = (char **)ft_calloc(sizeof(char *), (row + 1));
	if (!data->env)
		return ;
	while (j < row)
	{
		if (env_ky(envp, "SHLVL") && (j == env_find(envp, "SHLVL=")))
			data->env[i] = shlvl_update(envp);
		else if (env_ky(envp, "OLDPWD") && (j == env_find(envp, "OLDPWD=")))
			data->env[i] = ft_strdup(envp[++j]);
		else
			data->env[i] = ft_strdup(envp[j]);
		if (!data->env[i])
			return (free_char_2d(data->env));
		i++;
		j++;
	}
}

void	sig_quit(int signum)
{
	if (signum == SIGQUIT && g_signal == 0)
	{
		rl_redisplay();
	}
}

void	sig_int(int signum)
{
	if (signum == SIGINT && g_signal == 0)
	{
		ft_putchar_fd('\n', STDOUT_FILENO);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

static int	signal_init(void)
{
	t_sigaction	sig1;
	t_sigaction	sig2;

	if (sigemptyset(&sig1.sa_mask) == -1)
		return (0);
	if (sigemptyset(&sig2.sa_mask) == -1)
		return (0);
	sig1.sa_flags = SA_RESTART;
	sig1.__sigaction_u.__sa_handler = sig_int;
	sig2.sa_flags = SA_RESTART;
	sig2.__sigaction_u.__sa_handler = sig_quit;
	if (sigaction(SIGINT, &sig1, NULL) == -1)
		return (0);
	if (sigaction(SIGQUIT, &sig2, NULL) == -1)
		return (0);
	return (1);
}

int	mns_init(t_data *data, char **envp)
{
	g_signal = 0;
	data->errnum = 0;
	data->in_dbq = 0;
	data->env = 0;
	data->lst_token.head = 0;
	data->lst_token.tail = 0;
	data->grouped_token = 0;
	data->path_exec = 0;
	data->stdin_copy = 0;
	data->stdout_copy = 0;
	data->pid = 0;
	data->fd_in = 0;
	data->fd_out = 0;
	data->num_child = 0;
	data->index = 0;
	data->len_path = 0;
	data->builtin_parent = 0;
	if (!signal_init ())
		return (0);
	env_init(data, envp);
	data->env_row_max = row_count(data->env);
	return (1);
}
