#include "libft.h"

void	*ft_calloc(size_t num, size_t size)
{
	void	*ptr;

	if (num == SIZE_MAX || size == SIZE_MAX)
		return (NULL);
	ptr = (void *)malloc(num * size);
	if (ptr == NULL)
		return (NULL);
	if (ptr != NULL)
	{
		ft_bzero(ptr, num * size);
		return (ptr);
	}
	return (NULL);
}
