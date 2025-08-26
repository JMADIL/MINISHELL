/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajamoun <ajamoun@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 04:16:39 by ajamoun           #+#    #+#             */
/*   Updated: 2025/08/26 04:50:38 by ajamoun          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	ft_isredi(t_token *token)
{
	return (token->type == INPUT || token->type == OUTPUT
		|| token->type == HEREDOC || token->type == APPEND);
}

t_redi_list	*ft_redinew(t_token *token, bool expand)
{
	t_redi_list	*new;

	new = malloc(sizeof(t_redi_list));
	if (!new)
		return (NULL);
	new->type = token->type;
	new->file = ft_strdup(token->next->value);
	new->expand = expand;
	new->variable = token->next->variable;
	new->next = NULL;
	new->delim = NULL;
	new->content = NULL;
	new->tmp_fd = -1;
	new->heredoc_fd = -1;
	new->is_last = false;
	return (new);
}

void	ft_rediradd(t_redi_list **redi, t_redi_list *new)
{
	t_redi_list	*tmp;

	if (!*redi)
	{
		*redi = new;
		return ;
	}
	tmp = *redi;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}
