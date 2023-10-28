/*
gcc -I/opt/homebrew/Cellar/readline/8.2.1/include -L/opt/homebrew/Cellar/readline/8.2.1/lib -o mns mns.c libft/libft.a -lreadline -lncurses
gcc -I/usr/local/Cellar/readline/8.2.1/include -L/usr/local/Cellar/readline/8.2.1/lib -o mns mns.c libft/libft.a -lreadline -lncurses
*/

#include "minishell.h"

////////////////////////////mns_init.c////////////////////////////////////

void	mns_free(t_data *data)
{
	rl_clear_history();
}

////////////////////////////mns_init.c////////////////////////////////////

static void	sig_quit_handling(int sig_num)
{
	if (sig_num == SIGINT && g_signal == 0)
		rl_redisplay();
}

static void	sig_int_handling(int sig_num)
{
	if (sig_num == SIGINT && g_signal == 0)
	{
		ft_putchar_fd('\n', STDOUT_FILENO);
		rl_on_new_line();
		rl_replace_line("", STDOUT_FILENO);
		rl_redisplay();

	}
}

static void	signal_init (t_data *data)
{
	t_sigaction	sig_int;
	t_sigaction	sig_quit;

	sig_int.sa_handler = sig_int_handling;
	sigemptyset(&sig_int.sa_mask);
	sig_int.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sig_int, NULL);
	sig_quit.sa_handler = sig_quit_handling;
	sigemptyset(&sig_quit.sa_mask);
	sig_quit.sa_flags = SA_RESTART;
	sigaction(SIGQUIT, &sig_quit, NULL);
}



void	mns_init (t_data *data, char **envp)
{
	g_signal = 0;
	data->err_num = 0;
	signal_init (data);
	// env_int(data);
}

////////////////////////////minishell.c////////////////////////////////////

char	*ft_strjoin_free(char *src, char *dst)
{
	char	*res;
	size_t	count;
	size_t	i;
	size_t	j;

	if (!dst && !src)
		return (NULL);
	if (!dst || !*dst)
		return (src);
	else if (!src || !*src)
		return (dst);
	count = ft_strlen(src) + ft_strlen(dst);
	res = (char *)malloc(sizeof(char) * (count + 1));
	if (!res)
		return (NULL);
	i = 0;
	j = 0;
	while(src[j])
		res[i++] = src[j++];
	j = 0;
	while (dst[j])
		res[i++] = dst[j++];
	res[i] = '\0';
	return (free(src), free(dst), res);
}

static char	*input_msg_init (t_data *data)
{
	char	*path_color;
	char	*err_color;
	char	*input_msg;
	char	path_dir[PATH_MAX];

	getcwd(path_dir, sizeof(path_dir));
	path_color = ft_strjoin_free (ft_strdup("\x1b[34m"), ft_strdup(path_dir));
	err_color = ft_strjoin_free(ft_strdup("\x1b[33m?"), ft_itoa(data->err_num));
	input_msg = ft_strjoin_free(ft_strjoin_free(path_color, err_color), ft_strdup("\x1b[34m $\x1b[0m "));
	// input_msg = ft_strjoin_free(ft_strjoin_free (ft_strdup("\x1b[34m"), ft_strdup(path_dir)), ft_strdup("\x1b[34m $\x1b[0m "));
	if (!input_msg)
		input_msg = ft_strdup("minishell_input: ");
	return(input_msg);
}

static int	main_while (t_data *data)
{
	char	*input;
	char	*input_msg;

	input_msg = input_msg_init(data);
	input = readline(input_msg);
	free(input_msg);
	if (!input)
		return (1);
	if (!input[0])
	{
		free (input);
		return (0);
	}
	add_history(input);
	// input_to_token(data, input);
	// token_to_organize(data);

	// // *np-sam*
	// if (!(data->organized_token))
	// {
	// 	mns_free(data);
	// 	exit (0);
	// }

	return(0);

}


int main(int argc, char **argv, char **envp) 
{
	t_data	data;

	(void) argv;
	if (argc != 1)
		return (0);
	mns_init(&data, envp);
	while (1)
	{
		if(main_while(&data))
			break ;
		g_signal = 1;
		// excute_cmd(&data);
		g_signal = 0;
	}
	printf ("exit\n");
	mns_free(&data);
	return (0);
}

// ** debug **

	// if (input) {
	// 	printf("You entered: %s\n", input);
	// 	free(input); 
	// }
	
	// printf("%s", "checked");
	