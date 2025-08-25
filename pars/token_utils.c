/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajamoun <ajamoun@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 05:06:57 by ajamoun           #+#    #+#             */
/*   Updated: 2025/08/25 11:44:03 by ajamoun          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token	*ft_dollar_joining(t_token *token, t_token *new_token)
{
	char	*prefix;
	char	*value;

	if (ft_strlen(token->value) == 1)
		return (ft_free_token(token), new_token);
	else
	{
		prefix = ft_substr(token->value, 0, ft_strlen(token->value) - 1);
		value = ft_strjoin(prefix, new_token->value);
		free(prefix);
		ft_free_token(token);
		free(new_token->value);
		new_token->value = value;
		return (new_token);
	}
}

int	ft_toksize(t_token *lst)
{
	t_token	*p;
	int		len;

	if (lst == NULL)
		return (0);
	p = lst;
	len = 0;
	while (p != NULL)
	{
		p = p->next;
		len++;
	}
	return (len);
}
