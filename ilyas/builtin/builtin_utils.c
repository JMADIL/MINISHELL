/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irfei <irfei@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 02:13:42 by irfei             #+#    #+#             */
/*   Updated: 2025/08/22 02:13:43 by irfei            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../help.h"

/*
 * Creates and appends a new environment variable node to the env list.
 * If key is NULL or empty, does nothing. Value can be NULL.
 * Allocates memory for the node, assigns key/value ownership to the list.
 *
 * @param env: Pointer to environment list head
 * @param key: Key string (caller must strdup before passing)
 * @param value: Value string (caller must strdup before passing, can be NULL)
 * @return: 1 on success, 0 on failure (memory allocation error)
 */

// i use this function in unset.c blast "ft_lstadd_back"
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

/*
 * Creates a deep copy of the environment variable linked list.
 * Each node's key and value are duplicated so the new list is
 * completely independent from the original.
 *
 * @param env: Pointer to the head of the environment variables list
 * @return: Pointer to the head of the new copied list,
 *          or NULL if env is NULL or allocation fails
 * Side effects: Allocates memory for new list nodes and strings
 */

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

/*
 * Splits a string into a key and value at the first '=' character.
 * The key is the part before '=', and the value is the part after.
 * Memory is allocated for both parts, which must be freed by the caller.
 *
 * @param cmd: Full command string (e.g., "VAR=value")
 * @param key: Pointer to store allocated key string
 * @param value: Pointer to store allocated value string
 * @param equals_pos: Pointer to '=' inside cmd
 * Side effects: Allocates memory for *key and *value
 */
void	alloc_key_value(char *cmd, char **key, char **value, char *equals_pos)
{
	size_t	key_len;

	key_len = (size_t)(equals_pos - cmd);
	*key = ft_substr(cmd, 0, key_len);
	*value = ft_strdup(equals_pos + 1);
}
