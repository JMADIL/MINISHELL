/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irfei <irfei@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 02:13:44 by irfei             #+#    #+#             */
/*   Updated: 2025/08/24 08:26:38 by irfei            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	size_dp(char **c)
{
	int	count;

	count = 0;
	while (*c)
	{
		c++;
		count++;
	}
	return (count);
}

t_list	*find_node(t_list *env, char *key)
{
	t_list	*current_node;

	current_node = env;
	while (current_node)
	{
		if (ft_strcmp(current_node->key, key) == 0)
			return (current_node);
		current_node = current_node->next;
	}
	return (NULL);
}
