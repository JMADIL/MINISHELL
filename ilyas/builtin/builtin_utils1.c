/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irfei <irfei@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 02:13:44 by irfei             #+#    #+#             */
/*   Updated: 2025/08/22 02:13:45 by irfei            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../help.h"

// cd
int	size_dp(char **c)
{
	int	i;

	i = 0;
	while (*c)
	{
		c++;
		i++;
	}
	return (i);
}

t_list	*find_node(t_list *env, char *key)
{
	t_list *tmp;

	tmp = env;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, key) == 0)
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}