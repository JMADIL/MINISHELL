#include "../help.h"

int is_valide_identifier(const char *str)
{
	int i;

	if(!str || (!ft_isalpha(str[0]) && str[0] != '_'))
		return 0;
	i = 1;
	while(str[i] && str[i] != '-' && !(str[i] == '+' && str[i + 1] == '='))
	{
		if(!ft_isalnum(str[i]) && str[i] != '_')
			return 0;
		i++;
	}
	return 1;
}
