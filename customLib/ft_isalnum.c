#include "libft.h"

int	ft_isalnum(int c)
{
	if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z')
		|| (c >= 'a' && c <= 'z'))
	{
		return (1);
	}
	return (0);
}
/*
#include <ctype.h>
#include <stdio.h>
int main(void)
{
	char	str[] = "ABc++D123--";
	int	alnum = 0, ft = 0, i = 0;

	for (i = 0; str[i] != '\0'; i++)
	{
		if(isalnum(str[i]) != 0)
			alnum++;
		if(ft_isalnum(str[i]) != 0)
			ft++;
	}
	printf("isalnum = %d\n", alnum);
	printf("ft_isalnum = %d", ft);

}*/
