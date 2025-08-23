/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irfei <irfei@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 02:13:14 by irfei             #+#    #+#             */
/*   Updated: 2025/08/23 09:28:25 by irfei            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/* ================================================================
 * builtin_env - Print environment variables in KEY=VALUE format
 * 
 * Mimics the behavior of the 'env' command by displaying all
 * environment variables. Special handling for '_' variable to
 * show '/usr/bin/env' as expected by shell standards.
 * 
 * @param env: Double pointer to environment variable list
 * @return: 0 on success, 1 on error (sets g_exit_status)
 ================================================================ */

int	builtin_env(t_list **env, int out)
{
	t_list	*tmp;

	if (!env || !*env)
		return (g_exit_status = 1, 1);
	tmp = *env;
	while (tmp)
	{
		if (tmp->key && tmp->value)
		{
			if (ft_strcmp(tmp->key, "_") == 0)
			{
				ft_putstr_fd("_=/usr/bin/env\n", 1);
			}
			else
			{
				ft_putstr_fd(tmp->key, out);
				ft_putstr_fd("=", out);
				ft_putstr_fd(tmp->value, out);
				ft_putstr_fd("\n", out);
			}
		}
		tmp = tmp->next;
	}
	return (g_exit_status = 0, 0);
}
