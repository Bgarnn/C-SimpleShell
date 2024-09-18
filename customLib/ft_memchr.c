#include "libft.h"

void	*ft_memchr(const void *str, int val, size_t num)
{
	unsigned char	*ptr;

	ptr = (unsigned char *)str;
	while (num > 0)
	{
		if (*ptr == (unsigned char)val)
			return (ptr);
		ptr++;
		num--;
	}
	return (NULL);
}
// locate (val) in (str), within (num) byte