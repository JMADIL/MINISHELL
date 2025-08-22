/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajamoun <ajamoun@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 04:28:17 by ajamoun           #+#    #+#             */
/*   Updated: 2025/08/23 00:37:45 by ajamoun          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_get_varlen(char *str)
{
	int	i;

	i = 0;
	if (ft_strcmp(str, "?") == 0)
		return (1);
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	return (i);
}

char	*help_tmp2(t_token **token, ssize_t dollar_pos)
{
	return (ft_substr((*token)->value, dollar_pos
			+ ft_get_varlen((*token)->value + dollar_pos + 1) + 1,
			ft_strlen((*token)->value)));
}

bool	please_expand(t_token *tmp)
{
	return ((tmp->type == WORD || tmp->type == DOUBLE_QUOTE) && tmp->prev
		&& tmp->prev->type == HEREDOC);
}
