/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initenv.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajamoun <ajamoun@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 04:47:26 by ajamoun           #+#    #+#             */
/*   Updated: 2025/08/25 11:39:30 by ajamoun          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_list	*ft_initenv(char **env)
{
	t_list	*minienv;
	t_list	*new_node;
	char	*key;
	char	*value;
	int		i;

	minienv = NULL;
	if (!env || !*env)
		return (init_minimal_env((&minienv)), minienv);
	i = 0;
	while (env && env[i])
	{
		if (!ft_pars_env(env[i], &key, &value))
		{
			i++;
			continue ;
		}
		new_node = ft_lstnew(key, value);
		if (!new_node)
			return (free(key), free(value), NULL);
		ft_lstadd_back(&minienv, new_node);
		i++;
	}
	return (minienv);
}
