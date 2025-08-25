/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajamoun <ajamoun@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 04:28:54 by ajamoun           #+#    #+#             */
/*   Updated: 2025/08/25 11:33:36 by ajamoun          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	ft_redierrors(t_token *token)
{
	if (ft_isredi(token))
	{
		if (token->next == NULL)
			return (true);
		if (ft_isredi(token->next) || token->next->type == PIPE)
			return (true);
	}
	return (false);
}

bool	ft_pipeerrors(t_token *token)
{
	if (token->type == PIPE)
	{
		if (token->prev == NULL)
			return (true);
		if (token->next == NULL || token->next->type == PIPE)
			return (true);
	}
	return (false);
}
