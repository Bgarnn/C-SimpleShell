#include "libft.h"

void	ft_putendl_fd(char *str, int fd)
{
	if (str == NULL || fd < 0)
		return ;
	write(fd, str, ft_strlen(str));
	write(fd, "\n", 1);
}
// output (str) to the given (fd), followed by newline