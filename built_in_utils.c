#include "minishell.h"

void	check_sign(char *str, long *sign)
{
	while (*str == ' ' || *str == '\n' || *str == '\t' \
	|| *str == '\v' || *str == '\f' || *str == '\r')
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			*sign = -1;
		str++;
	}
}

long	ft_atol(char *str, int *status)
{
	unsigned long	sum;
	long			result;
	long			sign;
	size_t			is_neg;
	size_t			max_sum;

	*status = 0;
	sum = 0;
	sign = 1;
	is_neg = 0;
	check_sign(str, &sign);
	if (sign == -1)
		is_neg = 1;
	max_sum = (size_t)LONG_MAX + is_neg;
	while ((*str >= '0' && *str <= '9') && *status != 1)
	{
		sum = (sum * 10) + (*str - '0');
		if (sum > max_sum)
			*status = 1;
		str++;
	}
	result = sum;
	return (result * sign);
}

static int	check_digit(char *str, int index, unsigned char *status)
{
	while (str[index])
	{
		if (ft_isdigit(str[index]) == 0)
		{
			*status = 255;
			return (-1);
		}
		index++;
	}
	return (0);
}

int	check_status(t_token_node *curr_token, unsigned char *status)
{
	char	*str;
	int		index;
	int		is_overflow;

	str = curr_token->value;
	index = 0;
	if (str[0] != '+' && str[0] != '-' && !(ft_isdigit(str[0])))
		return (*status = 255, 1);
	if ((str[0] == '+' || str[0] == '-') && !str[1])
		return (*status = 255, 1);
	if (*str == '+' || *str == '-')
		index = 1;
	if (check_digit(str, index, status) == -1)
		return (1);
	if (check_digit(str, index, status) == 0 && curr_token->next != NULL)
		return (*status = 1, 2);
	*status = ft_atol(str, &is_overflow);
	return (is_overflow);
}
