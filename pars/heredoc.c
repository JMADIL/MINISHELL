/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irfei <irfei@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 05:17:48 by ajamoun           #+#    #+#             */
/*   Updated: 2025/08/24 10:26:22 by irfei            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*expand_inchar(char *value, t_list *minienv, ssize_t dollar_pos)
{
	char	*var;
	char	*expanded;

	var = ft_substr(value, dollar_pos + 1, ft_get_varlen(value + dollar_pos
				+ 1));
	expanded = ft_getvar(var, minienv);
	free(var);
	return (expanded);
}

void	expand_exit_status_inchar(char **value)
{
	ssize_t	dollar_pos;
	char	*expanded;
	char	*before_part;
	char	*after_part;
	char	*old_value;

	dollar_pos = dollar_position(*value);
	if ((*value)[dollar_pos + 1] == '?')
		expanded = ft_itoa(g_exit_status);
	else
		expanded = ft_strdup("");
	before_part = ft_strjoin_free(ft_substr(*value, 0, dollar_pos), expanded);
	after_part = ft_substr(*value, dollar_pos + 2, ft_strlen(*value));
	old_value = *value;
	*value = ft_strjoin_free(before_part, after_part);
	free(old_value);
	free(after_part);
	free(expanded);
}

bool	ft_condition_inchar(char *value, ssize_t dollar_pos)
{
	return (value[dollar_pos + 1] == '$' || value[dollar_pos + 1] == '?'
		|| ft_isdigit(value[dollar_pos + 1]));
}

char	*help_tmp2_inchar(char *value, ssize_t dollar_pos)
{
	return (ft_substr(value, dollar_pos + ft_get_varlen(value + dollar_pos + 1)
			+ 1, ft_strlen(value)));
}

void	expand_var_in_char(char	**value, t_list *minienv)
{
	ssize_t	dollar_pos;
	char	*expanded;
	char	*before_part;
	char	*after_part;
	char	*old_value;

	dollar_pos = dollar_position(*value);
	while (dollar_pos != -1)
	{
		if (ft_condition_inchar(*value, dollar_pos))
		{
			expand_exit_status_inchar(value);
			dollar_pos = dollar_position(*value);
			continue ;
		}
		expanded = expand_inchar(*value, minienv, dollar_pos);
		before_part = ft_strjoin_free(ft_substr(*value, 0, dollar_pos), expanded);
		after_part = help_tmp2_inchar(*value, dollar_pos);
		old_value = *value;
		*value = ft_strjoin_free(before_part, after_part);
		free(old_value);
		free(after_part);
		free(expanded);
		dollar_pos = dollar_position(*value);
	}
}
