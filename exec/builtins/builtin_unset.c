/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irfei <irfei@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 02:13:39 by irfei             #+#    #+#             */
/*   Updated: 2025/08/24 08:26:12 by irfei            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

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
	add_env_node(env, ft_strdup("OLDPWD"), NULL);
	add_env_node(env, ft_strdup("SHLVL"), ft_strdup("1"));
	add_env_node(env, ft_strdup("_"), ft_strdup("/usr/bin/env"));
	return (1);
}

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
		target = find_env_var(cmd[i], *env);
		if (target)
			remove_env_node(env, target);
		i++;
	}
	return (0);
}
