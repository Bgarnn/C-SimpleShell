#include "libft.h"

void	*ft_memset(void *str, int val, size_t num)
{
	unsigned char	*ptr;

	ptr = (unsigned char *)str;
	while (num > 0)
	{
		*ptr = (unsigned char)val;
		num--;
		ptr++;
	}
	return (str);
}
// fill (str) with (val), (num) byte