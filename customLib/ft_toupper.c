#include "libft.h"

int	ft_toupper(int c)
{
	if (c >= 'a' && c <= 'z')
	{
		return (c - 32);
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
		printf("%c", toupper(str[i]));
	}
    printf("\n");
    for (i = 0; str[i] != '\0'; i++)
	{
        printf("%c", ft_toupper(str[i]));
	}
}*/
