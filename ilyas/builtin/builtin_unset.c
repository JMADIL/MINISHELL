/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajamoun <ajamoun@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 02:13:39 by irfei             #+#    #+#             */
/*   Updated: 2025/08/22 22:05:41 by ajamoun          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/*
 * Removes a specific node from the environment variables linked list.
 * Handles proper unlinking of doubly-linked list node by updating
 * previous and next node pointers. Frees all allocated memory including
 * key, value strings and the node structure itself.
 *
 * @param env_list: Pointer to the head of environment list
 * @param node: List node to remove and free
 * @return: Always returns 1 (success indicator)
 * Side effects: Modifies linked list structure, frees memory
 */
int	remove_env_node(t_list **env_list, t_list *node)
{
	if (!node)
		return (1);
	if (!env_list)
		return (1);
	if (node->prev)
		node->prev->next = node->next;
	else
		*env_list = node->next;
	if (node->next)
		node->next->prev = node->prev;
	if (node->key)
		free(node->key);
	if (node->value)
		free(node->value);
	free(node);
	return (1);
}

/*
 * Initializes a minimal environment when none exists.
 * Creates essential shell environment variables including PWD (current working
 * directory), OLDPWD, SHLVL (shell level), and _ (last command). Used when
 * shell starts with empty environment or as fallback initialization.
 *
 * @param env: Pointer to environment variables linked list (should be empty)
 * @return: 1 if environment was initialized, 0 if environment already exists
 * Side effects: Allocates memory for environment nodes, calls getcwd()
 */
int	init_minimal_env(t_list **env)
{
	char	*cwd;
	char	cwd_buffer[1024];

	if (!env)
		return (0);
	if (*env)
		return (0);
	if (getcwd(cwd_buffer, sizeof(cwd_buffer)) != NULL)
		cwd = ft_strdup(cwd_buffer);
	else
		cwd = ft_strdup("/");
	add_env_node(env, ft_strdup("PWD"), cwd);
		// i can change it with ft_lstadd_back
	add_env_node(env, ft_strdup("OLDPWD"), NULL);
	add_env_node(env, ft_strdup("SHLVL"), ft_strdup("1"));
	add_env_node(env, ft_strdup("_"), ft_strdup("/usr/bin/env"));
	return (1);
}

/*
 * Implements the unset builtin command functionality.
 * Removes specified environment variables from the shell's environment.
 * Iterates through all provided variable names and removes matching entries
 * from the environment list. Protects special variables like '_' from removal.
 *
 * @param cmd: Array of variable names to unset (cmd[0] is "unset")
 * @param env: Pointer to environment variables linked list
 * @return: Always returns 1 (success indicator)
 * Side effects: Modifies environment list by removing specified variables
 */
int	builtin_unset(char **cmd, t_list **env)
{
	int		i;
	t_list	*target;

	if (!cmd || !cmd[0] || !env || !*env)
		return (1);
	i = 1;
	while (cmd[i])
	{
		if (ft_strcmp(cmd[i], "_") == 0)
		{
			i++;
			continue ;
		}
		target = find_env_var(cmd[i], *env); // from builtin_export.c
		if (target)
			remove_env_node(env, target);
		i++;
	}
	return (0);
}
