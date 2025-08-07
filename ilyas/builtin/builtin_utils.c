#include "../help.h"

char *ft_strdup(char *str)
{
	int i = 0;
	int l = 0;
	while (str[l])
		l++;
	char *t = malloc(l + 1);
	
	if(!t)
		return NULL;

	while (str[i])
	{
		t[i] = str[i];
		i++;
	}
	t[i] = '\0';
	return t;
}

int ft_strlen(char *l)
{
	int i;
	if(!l)
		return 0;
	i = 0;
	while (l[i])
		i++;
	return i;
}