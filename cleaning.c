/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaning.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irfei <irfei@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 06:16:59 by ajamoun           #+#    #+#             */
/*   Updated: 2025/08/23 05:09:37 by irfei            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_close_pipe(t_redi_list *redi)
{
	while (redi)
	{
		if (redi->heredoc_fd != -1)
		{
			close(redi->heredoc_fd);
			redi->heredoc_fd = -1;
		}
		redi = redi->next;
	}
}

void	ft_cleaner(t_token *token_list, t_cmdarg *cmdarg_list)
{
	t_cmdarg	*tmp;

	tmp = cmdarg_list;
	while (tmp)
	{
		if (tmp->redirections)
			ft_close_pipe(tmp->redirections);
		tmp = tmp->next;
	}
	ft_free_tokenlist(token_list);
	ft_free_cmdlist(cmdarg_list);
}
