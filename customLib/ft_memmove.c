#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t num)
{
	char	*ptrs;
	char	*ptrd;

	ptrs = (char *)src;
	ptrd = (char *)dst;
	if (dst > src)
	{
		while (num > 0)
		{
			num--;
			ptrd[num] = ptrs[num];
		}
		return (dst);
	}
	else
		return (ft_memcpy(dst, src, num));
}
// copy from (src) to (dst), (num) byte, can overlap