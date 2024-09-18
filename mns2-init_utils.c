#include "minishell.h"

char	*env_ky(char **env, char *key)
{
	size_t	i;
	size_t	str_len;
	size_t	key_len;

	i = 0;
	key_len = ft_strlen(key);
	while (env[i])
	{
		str_len = index_of_c(env[i], '=');
		if (key_len > str_len)
			str_len = key_len;
		if (ft_strncmp(key, env[i], str_len) == 0)
			return (ft_strchr(env[i], '=') + 1);
		i++;
	}
	return (NULL);
}

char	*shlvl_update(char **envp)
{
	int	shlvl_num;

	shlvl_num = ft_atoi(env_ky(envp, "SHLVL"));
	if (shlvl_num)
		shlvl_num++;
	return (strjoin_f(ft_strdup("SHLVL="), ft_itoa(shlvl_num)));
}

int	env_find(char **envp, char *key)
{
	int	i;
	int	row_shlvl;

	i = 0;
	row_shlvl = 0;
	while (envp[i])
	{
		if (ft_strncmp(key, envp[i], ft_strlen(key)) == 0)
			break ;
		if (ft_strncmp(key, envp[i], ft_strlen(key)) != 0)
			row_shlvl++;
		i++;
	}
	return (row_shlvl);
}

int	row_count(char **str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

void	group_init(t_group_ptr	*group)
{
	group->infile.head = 0;
	group->infile.tail = 0;
	group->outfile.head = 0;
	group->outfile.tail = 0;
	group->cmd.head = 0;
	group->cmd.tail = 0;
}
