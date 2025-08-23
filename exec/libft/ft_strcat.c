
#include "../help.h"
char	*ft_strcat(char *dest, const char *src)
{
	char	*original_dest;

	if (!dest || !src)
		return (dest);
	original_dest = dest;
	while (*dest)
		dest++;
	while (*src)
	{
		*dest = *src;
		dest++;
		src++;
	}
	*dest = '\0';
	return (original_dest);
}