#include "libft.h"

int	ft_tolower(int c)
{
	if (c >= 'A' && c <= 'Z')
	{
		return (c + 32);
	}
	return (c);
}
/*
#include <ctype.h>
#include <stdio.h>
int main(void)
{
	char	str[] = "asdf**UJDHF";
	int	i;

	for (i = 0; str[i] != '\0'; i++)
	{
		printf("%c", tolower(str[i]));
	}
	printf("\n");
	for (i = 0; str[i] != '\0'; i++)
	{
		printf("%c", ft_tolower(str[i]));
	}
}*/
