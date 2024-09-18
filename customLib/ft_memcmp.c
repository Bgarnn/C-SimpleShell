#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t num)
{
	unsigned char	*ptr1;
	unsigned char	*ptr2;
	size_t			i;

	ptr1 = (unsigned char *)s1;
	ptr2 = (unsigned char *)s2;
	i = 0;
	while (num > 0)
	{
		if (ptr1[i] != ptr2[i])
			return ((unsigned char)ptr1[i] - ptr2[i]);
		num--;
		i++;
	}
	return (0);
}
// compare byte srings, both are assumed to be (num) byte long