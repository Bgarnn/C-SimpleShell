#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t num)
{
	char	*ptrs;
	char	*ptrd;

	ptrs = (char *)src;
	ptrd = (char *)dst;
	if (ptrs != NULL || ptrd != NULL)
	{
		while (num > 0)
		{
			*ptrd = *ptrs;
			num--;
			ptrs++;
			ptrd++;
		}
	}
	return (dst);
}
// copy from (src) to (dst), (num) byte, cannot overlap