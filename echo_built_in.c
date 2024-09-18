#include "minishell.h"

static int	flag_check(t_token_node *curr_token)
{
	size_t	i;

	if (curr_token->value[0] != '-')
		return (FALSE);
	i = 1;
	while (curr_token->value[i])
	{
		if (curr_token->value[i] != 'n')
			return (FALSE);
		i++;
	}
	return (TRUE);
}

static void	echo_no_flag(t_token_node *curr_token)
{
	while (curr_token != NULL)
	{
		if (curr_token->next == NULL)
		{
			ft_putendl_fd(curr_token->value, 1);
			break ;
		}
		ft_putstr_fd(curr_token->value, 1);
		if (curr_token->next != NULL)
			ft_putstr_fd(" ", 1);
		curr_token = curr_token->next;
	}
}

void	echo_words(t_data *data, t_token_node *curr_token)
{
	if (curr_token == NULL)
	{
		printf("\n");
		exit (0);
	}
	else if (flag_check(curr_token) == FALSE)
		echo_no_flag(curr_token);
	else if (curr_token->next != NULL)
	{
		curr_token = curr_token->next;
		while (curr_token != NULL)
		{
			ft_putstr_fd(curr_token->value, 1);
			if (curr_token->next != NULL)
				ft_putstr_fd(" ", 1);
			curr_token = curr_token->next;
		}
	}
	data->errnum = 0;
	return ;
}
