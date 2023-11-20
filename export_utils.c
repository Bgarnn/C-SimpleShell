/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plertsir <plertsir@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 20:24:24 by plertsir          #+#    #+#             */
/*   Updated: 2023/11/16 15:44:12 by plertsir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid_ident(t_data *data, t_token_node *curr_token)
{
	int	i;

	(void)data;
	i = 0;
	if (ft_isalpha(curr_token->value[0]) == 0 && (curr_token->value[i] != '_'))
		return (FALSE);
	while (curr_token->value[i])
	{
		if (curr_token->value[i] == '=')
			return (TRUE);
		if (ft_isalnum(curr_token->value[i]) == 0 && \
		(curr_token->value[i] != '_'))
			return (FALSE);
		i++;
	}
	return (TRUE);
}

void	free_env(char **env_copy)
{
	size_t	i;

	i = 0;
	while (env_copy[i])
	{
		free(env_copy[i]);
		env_copy[i] = NULL;
		i++;
	}
	free(env_copy);
	env_copy = NULL;
}

char	*join_quote(char *env_copy)
{
	int		i;
	char	*tmp;
	char	*combine;
	char	*front;
	char	*back;

	i = 0;
	while (env_copy[i] != '=')
		i++;
	tmp = ft_strchr(&env_copy[0], '=');
	front = ft_substr(&env_copy[0], 0, tmp - &env_copy[0] + 1);
	back = ft_substr(&env_copy[0], i + 1, ft_strlen(env_copy) - (size_t)tmp);
	combine = ft_strjoin("\"", back);
	free(back);
	tmp = combine;
	combine = ft_strjoin(combine, "\"");
	free(tmp);
	tmp = combine;
	combine = ft_strjoin(front, combine);
	free(tmp);
	free(front);
	free(env_copy);
	return (combine);
}

void	sort_env(t_data *data, char **env_copy)
{
	size_t	i;
	size_t	j;
	char	*tmp;

	i = 0;
	while (i < data->env_row_max - 1)
	{
		j = 0;
		while (j < data->env_row_max - i - 1)
		{
			if (ft_strcmp(env_copy[j], env_copy[j + 1]) > 0)
			{
				tmp = env_copy[j];
				env_copy[j] = env_copy[j + 1];
				env_copy[j + 1] = tmp;
			}
			j++;
		}
		i++;
	}
}

size_t	len_env(char *str)
{
	size_t	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i] && str[i] != '=')
	{
		i++;
	}
	return (i);
}
