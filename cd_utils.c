/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plertsir <plertsir@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 16:22:23 by plertsir          #+#    #+#             */
/*   Updated: 2023/11/15 18:45:27 by plertsir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_home(t_data *data)
{
	int	i;

	i = 0;
	while (data->env[i])
	{
		if (is_duplicate(data->env[i], "HOME=") == 1)
			return (data->env[i]);
		i++;
	}
	return (NULL);
}
