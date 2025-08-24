/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irfei <irfei@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 02:13:14 by irfei             #+#    #+#             */
/*   Updated: 2025/08/24 08:07:32 by irfei            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

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
