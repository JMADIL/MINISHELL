/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajamoun <ajamoun@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 04:49:59 by ajamoun           #+#    #+#             */
/*   Updated: 2025/08/26 16:36:07 by ajamoun          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	extend_cmd_for_word(t_cmdarg **node)
{
	if ((*node)->cmdsize >= (*node)->cmd_capacity)
	{
		if (!resize_cmd_array(node, (*node)->cmd_capacity * 2 + 1))
		{
			perror("Failed to resize cmd array");
			g_exit_status = 1;
			return (0);
		}
	}
	return (1);
}

static void	add_or_join_word(t_cmdarg **node, t_token *token_list, char *value)
{
	char	*old_cmd;

	if (token_list->current->prev != NULL
		&& token_list->current->prev->addspace == true)
		(*node)->cmd[(*node)->cmdsize++] = value;
	else
	{
		if ((*node)->cmdsize > 0)
		{
			old_cmd = (*node)->cmd[(*node)->cmdsize - 1];
			(*node)->cmd[(*node)->cmdsize - 1] = ft_strjoin(old_cmd, value);
			free(old_cmd);
			free(value);
		}
		else
			(*node)->cmd[(*node)->cmdsize++] = value;
	}
}

void	*parsing_word(t_cmdarg **node, t_token *token_list)
{
	char	*value;

	value = ft_strdup(token_list->current->value);
	if (!value)
		return (perror("malloc failure"), g_exit_status = 1, NULL);
	if (token_list->current->variable == true
		&& token_list->current->type == WORD)
	{
		handle_split_word(node, token_list, value);
	}
	else
	{
		if (!extend_cmd_for_word(node))
			return (free(value), NULL);
		add_or_join_word(node, token_list, value);
	}
	return (NULL);
}

void	parsing_redi(t_cmdarg **node, t_token *token_list)
{
	t_token		*next;
	t_redi_list	*new_redi;

	if (token_list->current->type == INPUT
		|| token_list->current->type == HEREDOC)
	{
		if (token_list->current->next->type == WORD
			|| token_list->current->next->type == DOUBLE_QUOTE
			|| token_list->current->next->type == SINGLE_QUOTE)
		{
			new_redi = ft_redinew(token_list->current, true);
			ft_rediradd(&(*node)->redirections, new_redi);
			next = token_list->current->next;
			new_redi->delim = ft_strdup(next->value);
		}
		else
		{
			ft_rediradd(&(*node)->redirections, ft_redinew(token_list->current,
					false));
		}
	}
	else
		ft_rediradd(&(*node)->redirections, ft_redinew(token_list->current,
				true));
	token_list->current = token_list->current->next;
}
