/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajamoun <ajamoun@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 02:13:39 by irfei             #+#    #+#             */
/*   Updated: 2025/08/25 08:57:23 by ajamoun          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int    remove_env_node(t_list **env_list, int i)
{
	t_list *current;
	int index = 0;
	if (!env_list || !*env_list)
		return (1);
	current = *env_list;
	while (current && index < i)
	{
		current = current->next;
		index++;
	}
	if (!current) // i is out of bounds
		return (1);
	if (current->prev)
		current->prev->next = current->next;
	else
		*env_list = current->next;
	if (current->next)
		current->next->prev = current->prev;
	if (current->key)
		free(current->key);
	if (current->value)
		free(current->value);
	free(current);
	return (1);
}

int    init_minimal_env(t_list **env)
{
	char    *cwd;
	char    cwd_buffer[1024];

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

int    builtin_unset(char **cmd, t_list **env)
{
	int        i;
	t_list    *target;

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
			remove_env_node(env, i);
		i++;
	}
	return (0);
}
