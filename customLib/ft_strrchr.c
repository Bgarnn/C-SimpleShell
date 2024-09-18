#include "libft.h"

char	*ft_strrchr(const char *str, int val)
{
	char	*ptr;
	int		i;

	ptr = (char *)str;
	i = ft_strlen(str);
	if (val == '\0')
		return ((ptr) + i);
	while (i >= 0)
	{
		if (ptr[i] == (char)val)
			return ((ptr) + i);
		i--;
	}
	return (NULL);
}
// locate first (val) in (str), NULL-terminate