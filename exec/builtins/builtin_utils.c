/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irfei <irfei@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 02:13:42 by irfei             #+#    #+#             */
/*   Updated: 2025/08/24 08:11:32 by irfei            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	add_env_node(t_list **env, char *key, char *value)
{
	t_list	*new_node;
	t_list	*tmp;

	if (!env || !key || key[0] == '\0')
		return (0);
	new_node = malloc(sizeof(t_list));
	if (!new_node)
		return (0);
	new_node->key = key;
	new_node->value = value;
	new_node->check = 0;
	new_node->prev = NULL;
	new_node->next = NULL;
	if (!*env)
	{
		*env = new_node;
		return (1);
	}
	tmp = *env;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_node;
	new_node->prev = tmp;
	return (1);
}

static int	copy_env_node(t_list **copy, t_list *node)
{
	char	*key_copy;
	char	*val_copy;

	if (node->key)
		key_copy = ft_strdup(node->key);
	else
		key_copy = ft_strdup("");
	if (node->value)
		val_copy = ft_strdup(node->value);
	else
		val_copy = NULL;
	if (!key_copy || (node->value && !val_copy))
	{
		free(key_copy);
		free(val_copy);
		return (-1);
	}
	ft_lstadd_back(copy, ft_lstnew(key_copy, val_copy));
	return (0);
}

t_list	*copy_env_list(t_list *env)
{
	t_list	*copy;
	t_list	*tmp;

	if (!env)
		return (NULL);
	copy = NULL;
	tmp = env;
	while (tmp)
	{
		if (copy_env_node(&copy, tmp) == -1)
		{
			free_env_list(&copy);
			return (NULL);
		}
		tmp = tmp->next;
	}
	return (copy);
}

void	alloc_key_value(char *cmd, char **key, char **value, char *equals_pos)
{
	size_t	key_len;

	key_len = (size_t)(equals_pos - cmd);
	*key = ft_substr(cmd, 0, key_len);
	*value = ft_strdup(equals_pos + 1);
}
