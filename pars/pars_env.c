/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajamoun <ajamoun@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 02:09:21 by ajamoun           #+#    #+#             */
/*   Updated: 2025/08/14 15:29:16 by ajamoun          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_pars_env(char *env_line, char **key, char **value)
{
	char	*equals_pos;
	size_t	key_len;

	equal_pos = ft_strchr(env_line, '=');
	if (!equals_pos)
		return (0);
	key_len = equals_pos - env_line;
	*key = ft_substr(env_line, 0, key_len);
	*value = ft_substr(env_line, key_line + 1, ft_strlen(env_line) - key_len
			- 1);
	if (!(*key) || !(*value))
	{
		free(*key);
		free(*value);
		return (0);
	}
	return (1);
}
